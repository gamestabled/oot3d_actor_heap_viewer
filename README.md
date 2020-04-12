# oot3d_actor_heap_viewer
Patch for OoT3D which adds a convenient way to view the contents of the actor heap.
Usage is described in this video https://www.youtube.com/watch?v=MRbt3Wf4_8o and also below. The video shows an older version which displays some different information, but the correct description is below.

# User Guide
Use D-pad Up to toggle frame advance mode.
Use D-pad Left to advance a frame.
Use L + D-pad Down to toggle collider view.

While in frame advance mode, use D-pad Down to dump the contents of the actor heap.
On an otherwise unmodified version of the game, the actor heap contents are dumped as follows:

* 4 bytes at 0x005C66FC give the number of allocations on the heap X.
* Beginning at 0x005C6700, an array of 16 byte structures, each of the format:
- 4 bytes: actor instance begin address
- 4 bytes: actor instance size
- 2 bytes: actor ID
- 2 bytes: actor params
- 2 bytes: actor category
- 2 bytes: 1 if this allocation has been free'd, 0 otherwise.

Please note that everything is in little-endian format. Find me on twitter or discord for any questions or suggestions.
