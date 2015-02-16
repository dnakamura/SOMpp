#include <memory/Heap.h>
#include <memory/GenerationalHeap.h>
#include <memory/GenerationalCollector.h>
#include <memory/GenerationalPage.h>

#include <vmobjects/VMObjectBase.h>
#include <vmobjects/AbstractObject.h>
#include <vmobjects/VMInteger.h>
#include <vmobjects/IntegerBox.h>
#include <vm/Universe.h>

#include <string.h>
#include <iostream>

using namespace std;

GenerationalHeap::GenerationalHeap(size_t pageSize, size_t objectSpaceSize)
    : Heap<GenerationalHeap>(new GenerationalCollector(this)),
      pagedHeap(this, pageSize, objectSpaceSize / pageSize),
      sizeOfMatureObjectHeap(0),
      allocatedObjects(new vector<AbstractVMObject*>()) {}

NurseryPage* GenerationalHeap::RegisterThread() {
    return pagedHeap.GetNextPage();
}

void GenerationalHeap::UnregisterThread(NurseryPage* page) {
    // NOOP, because we the page is still in use
}

void GenerationalHeap::writeBarrier_OldHolder(AbstractVMObject* holder,
                                              const vm_oop_t referencedObject) {
    AbstractVMObject* obj = reinterpret_cast<AbstractVMObject*>(referencedObject);
    if (obj->GetGCField() & MASK_OBJECT_IS_OLD) {
        return;
    }
    
    NurseryPage* page = getPageFromObj(reinterpret_cast<AbstractVMObject*>(referencedObject));

    page->oldObjsWithRefToYoungObjs.push_back(holder);
    holder->SetGCField(holder->GetGCField() | MASK_SEEN_BY_WRITE_BARRIER);
}

void* NurseryPage::allocateInNextPage(size_t size ALLOC_OUTSIDE_NURSERY_DECLpp) {
    assert(interpreter);
    
    if (next == nullptr) {
        next = heap->pagedHeap.GetNextPage();
        next->SetInterpreter(interpreter);
        
        // need to set the page unconditionally, even if it is not yet
        // completely full. Otherwise, we can have the situation that during
        // the copying phase, an object gets moved to a previous page, and
        // is missed while moving all dependent objects, and thus, not moving
        // its dependent objects.
        interpreter->SetPage(reinterpret_cast<Page*>(next));
    }
    
    return next->AllocateObject(size ALLOC_HINT);
}

void NurseryPage::WalkObjects(walk_heap_fn walk,
                                               Page* target) {
    AbstractVMObject* curObject = reinterpret_cast<AbstractVMObject*>(buffer);
    
    while (curObject < nextFreePosition) {
        curObject->WalkObjects(walk, target);
        curObject = curObject->GetNextObject();
    }
}

static void invalidate_objects(gc_oop_t* oop, Page*) {
    if (IS_TAGGED(*oop) || *oop == nullptr) {
        return;
    }
    
    AbstractVMObject* obj = AS_OBJ(*oop);
    obj->MarkObjectAsInvalid();
}

void NurseryPage::Reset() {
    next             = nullptr;
    interpreter      = nullptr;
    nextFreePosition = buffer;
    
    if (DEBUG) {
        //        memset(buffer, 0, heap->pageSize);
        WalkObjects(invalidate_objects, nullptr);
    }
    
    oldObjsWithRefToYoungObjs.clear();
}
