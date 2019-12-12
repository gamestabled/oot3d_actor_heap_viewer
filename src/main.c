#include "z3d.h"
#include "heap.h"
#include "advance.h"

z3d_xyz_t pos;
z3d_rot_t rot;

advance_ctx_t advance_ctx = {};
advance_input_t inputs = {};
uint8_t advance_init;

void toggle_advance();
uint32_t set_actor_list(heap_node_t *begin);

void scan_inputs() {
    inputs.cur.val = real_hid.pad.pads[real_hid.pad.index].curr.val;
    inputs.pressed.val = (inputs.cur.val) & (~inputs.old.val);
    inputs.up.val = (~inputs.cur.val) & (inputs.old.val);
    inputs.old.val = inputs.cur.val;
}

void store_pos() {
    pos = link.pos_1;
    rot = link.rot_1;
}

void restore_pos() {
    link.pos_1 = pos;
    link.pos_2 = pos;
    link.rot_1 = rot;
    link.rot_2 = rot;
    link.rot_3 = rot;
}

void advance_main() {

    scan_inputs();

    /* (Re)store Position code
    ---------------------------*/
    //Stores Link's position
    if(inputs.pressed.d_left && inputs.cur.l) {
        store_pos();
    }

    //Restores Link's position
    if(inputs.pressed.d_right) {
        restore_pos();
    }

    /* Actor collision code */
    //Toggles actor collision viewer
    if(inputs.pressed.d_down && inputs.cur.l) {
        actor_collision = 1 - actor_collision;
    }

    /*Frame Advance begins here
    Stolen from n3rdswithgame <3
    ---------------------------*/
    if(advance_init == 0) {
        advance_init = 1;
    }

    //up to enable, left to advance
    toggle_advance();
    scan_inputs();


    if(advance_ctx.advance_state == STEP) {
        if(inputs.cur.d_left && !inputs.cur.l) {
            advance_ctx.advance_state = LATCHED;
        } else {
            advance_ctx.advance_state = PAUSED;
        }
    }


    while(advance_ctx.advance_state == PAUSED || advance_ctx.advance_state == LATCHED) {
        scan_inputs();
        toggle_advance();
        if(inputs.pressed.d_down) {
            set_actor_list((heap_node_t *)(*((void**)actor_heap_begin_addr_maybe)));
        }
        if(advance_ctx.advance_state == LATCHED && !inputs.cur.d_left) {
            advance_ctx.advance_state = PAUSED;
        }
        if(advance_ctx.advance_state == PAUSED && inputs.cur.d_left) {
            advance_ctx.advance_state = STEP;
        }
        svcSleepThread(16e6);
    }

}

uint32_t set_actor_list(heap_node_t *begin){
    heap_node_t *cur = begin;
    uint32_t count = 0;
    actor_data cur_data;
    while (cur){
        cur_data.heap_node_begin = cur;
        cur_data.actor_instance_begin = cur + 1;
        cur_data.size = cur->size;
        cur_data.category = cur_data.actor_instance_begin->type;
        cur_data.free = cur->free;
        actor_list[count] = cur_data;
        count++;
        cur = cur->next;
    }
    actor_list[count].heap_node_begin = NULL;
    actor_list[count].actor_instance_begin = NULL;
    actor_list[count].size = 0;
    actor_list[count].category = 0;
    actor_list[count].free = 0;
    num_actors = count;
    return count;
}

void area_load_main(){}

//TODO: change d_down_latched to d_up_latched or whatever final button code is
void toggle_advance() {
    //scan_inputs();
    if(inputs.pressed.d_up && advance_ctx.advance_state == NORMAL && !advance_ctx.d_down_latched){
        advance_ctx.advance_state = PAUSED;
        advance_ctx.d_down_latched = 1;
    } else if(inputs.pressed.d_up && advance_ctx.advance_state != NORMAL && !advance_ctx.d_down_latched) {
        advance_ctx.advance_state = NORMAL;
        advance_ctx.d_down_latched = 1;
    } else if (advance_ctx.advance_state == NORMAL && inputs.pressed.d_left && !inputs.cur.l){
        advance_ctx.advance_state = LATCHED;
    } else if(!inputs.pressed.d_up) {
        advance_ctx.d_down_latched = 0;
    }
}
