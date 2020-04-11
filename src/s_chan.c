#include "s_task.h"
#include <string.h>

/*******************************************************************/
/* chan                                                            */
/*******************************************************************/

/* Put element into chan */
void s_chan_put(__async__, s_chan_t *chan, const void *in_object) {
    uint16_t end;
    while (chan->available_count >= chan->max_count) {
        s_event_wait(__await__, &chan->event);
    }

    end = chan->begin + chan->available_count;
    while (end > chan->max_count)
        end -= chan->max_count;

    memcpy((char*)&chan[1] + end * chan->element_size, in_object, chan->element_size);
    ++chan->available_count;

    s_event_set(&chan->event);
}

/* Get element from chan */
void s_chan_get(__async__, s_chan_t *chan, void *out_object) {
    while (chan->available_count <= 0) {
        s_event_wait(__await__, &chan->event);
    }

    memcpy(out_object, (char*)&chan[1] + chan->begin * chan->element_size, chan->element_size);

    ++chan->begin;
    while (chan->begin > chan->max_count)
        chan->begin -= chan->max_count;
    --chan->available_count;

    s_event_set(&chan->event);
}
