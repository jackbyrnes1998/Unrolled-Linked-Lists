# Unrolled-Linked-Lists
Each Node object contains a fxed size array (size = 6 in the above example) that will store 1 or more elements
from the list. The elements are ordered from left to right. From the outside, this unrolled linked list should
perform exactly like an STL list containing the numbers 10 through 23 in sorted order. Note that to match
the behavior, the list_iterator object must also change. The iterator must keep track of not only which
Node it refers to, but also which element within the Node it's on. This can be done with a simple offset index.
In the above example, the iterator refers to the element \19".
Just like regular linked lists, the unrolled linked list supports speedy erase operations from the middle of
the list. For example, if we call erase with the iterator shown above, this is the resulting picture:

Note that the size of the list has decreased by one, the element 19 has been removed from the middle
node, the elements after 19 have been shifted to the left, and the number of elements in that node has been
decreased. It's perfectly OK for some of the nodes to be only partially flled. (If we tried to enforce that
all nodes stay full, this operation would become just as expensive as the STL vector::erase operation!) If
removing an element causes a node to become empty, the entire node should be removed from the list.

