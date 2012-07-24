//
//  test_heap.c
//  Firedrake
//
//  Created by Sidney Just
//  Copyright (c) 2012 by Sidney Just
//  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
//  documentation files (the "Software"), to deal in the Software without restriction, including without limitation 
//  the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
//  and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
//  INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
//  PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE 
//  FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
//  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#include <memory/memory.h>
#include "unittests.h"

void _test_integrity();
void _test_free();
void _test_fragmentation();

#define _test_heap_size 1024 * 1024
extern void heap_defrag(heap_t *heap);

void test_heap()
{
	kunit_test_suite_t *heapSuite = kunit_test_suiteCreate("Heap Tests", "A test suite for testing the heap allocator", true);
	{
		kunit_test_suiteAddTest(heapSuite, kunit_testCreate("Integrity test", "Tests the integrity of the heap allocator", _test_integrity));
		kunit_test_suiteAddTest(heapSuite, kunit_testCreate("Free test", "Tests if free works correctly", _test_free));
		kunit_test_suiteAddTest(heapSuite, kunit_testCreate("Fragmentions test", "Tests if the defragmention works correctly", _test_fragmentation));
	}
	kunit_test_suiteRun(heapSuite);
}

// Helper functions

size_t _test_heap_numberOfAllocations(heap_t *heap)
{
	size_t count = 0;
	struct _heap_allocation_s *allocation = heap->firstAllocation;

	while(allocation)
	{
		count ++;
		allocation = allocation->next;
	}

	return count;
}

// Test implementations

void _test_integrity()
{
	heap_t *heap = heap_create(_test_heap_size, vm_getKernelDirectory(), 0, VM_FLAGS_KERNEL);
	KUAssertNotNull(heap, "We need a heap to test!");

	uintptr_t pptr = 0x0;
	for(int i=0; i<10; i++)
	{
		uintptr_t ptr = (uintptr_t)halloc(heap, 64);
		if(pptr != 0x0)
		{
			size_t diff = ptr - pptr;
			KUAssertTrue(diff >= 64 + sizeof(struct _heap_allocation_s), "Allocation %i isn't large enough!", i);
		}
	}

	heap_destroy(heap);
}

void _test_free()
{
	heap_t *heap = heap_create(_test_heap_size, vm_getKernelDirectory(), 0, VM_FLAGS_KERNEL);
	KUAssertNotNull(heap, "We need a heap to test!");

	void *pointer = NULL;
	for(int i=0; i<10; i++)
	{
		void *npointer = halloc(heap, 64);
		hfree(heap, npointer);

		if(pointer)
			KUAssertEquals(pointer, npointer, "");

		pointer = npointer;
	}

	heap_destroy(heap);
}

void _test_fragmentation()
{
	heap_t *heap = heap_create(_test_heap_size, vm_getKernelDirectory(), 0, VM_FLAGS_KERNEL);
	KUAssertNotNull(heap, "We need a heap to test!");

	void *ptr[20];
	for(int i=0; i<20; i++)
	{
		ptr[i] = halloc(heap, 64);
	}

	KUAssertEquals(_test_heap_numberOfAllocations(heap), 21, "The heap must have 21 allocations!"); // 21 because there is one large free allocation next to our 20 allocations

	// Free all even allocations
	for(int i=0; i<20; i++)
	{
		if(!(i % 2))
		{
			hfree(heap, ptr[i]);
		}
	}



	heap_defrag(heap); // Force a defragmentation!
	KUAssertEquals(_test_heap_numberOfAllocations(heap), 21, "The heap must still have 21 allocations!");


	// Free all odd allocations
	for(int i=0; i<20; i++)
	{
		if((i % 2))
		{
			hfree(heap, ptr[i]);
		}
	}

	heap_defrag(heap); // Force a defragmentation!
	KUAssertEquals(_test_heap_numberOfAllocations(heap), 1, "The heap must only have 1 allocation left!");

	heap_destroy(heap);
}
