#include "IntegerBox.h"
#include "VMInteger.h"

#include <vm/Universe.h>

GCInteger* GlobalBox::integerBox = nullptr;

void GlobalBox::updateIntegerBox(VMInteger* newValue) {
    integerBox = to_gc_ptr(newValue);
}

VMInteger* GlobalBox::IntegerBox() {
    return load_ptr(integerBox);
}

void GlobalBox::WalkGlobals(walk_heap_fn walk, Page* page) {
    integerBox = static_cast<GCInteger*>(walk(integerBox, page));
}
