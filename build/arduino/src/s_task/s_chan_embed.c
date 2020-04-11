#include "s_task.h"
#include <string.h>

#ifdef USE_IN_EMBEDDED

/*******************************************************************/
/* chan for communication between irq and task                     */
/*******************************************************************/

/* Put element into chan and wait interrupt to read the chan */
void s_chan_put__to_irq(__async__, s_chan_t *chan, const void *in_object) {
    uint16_t end;
    
    S_IRQ_DISABLE();

    while (chan->available_count >= chan->max_count) {
        s_event_wait__from_irq(__await__, &chan->event);
    }

    end = chan->begin + chan->available_count;
    while (end > chan->max_count)
        end -= chan->max_count;

    memcpy((char*)&chan[1] + end * chan->element_size, in_object, chan->element_size);
    ++chan->available_count;
    
    S_IRQ_ENABLE();
}

/* Wait interrupt to write the chan and then get element from chan */
void s_chan_get__from_irq(__async__, s_chan_t *chan, void *out_object) {
    S_IRQ_DISABLE();

    while (chan->available_count <= 0) {
        s_event_wait__from_irq(__await__, &chan->event);
    }

    memcpy(out_object, (char*)&chan[1] + chan->begin * chan->element_size, chan->element_size);

    ++chan->begin;
    while (chan->begin > chan->max_count)
        chan->begin -= chan->max_count;
    --chan->available_count;

    S_IRQ_ENABLE();
}


/*
 * Interrupt writes element into the chan
 *  return 0 on chan element was written
 *  return -1 on chan is full
 */
int s_chan_put__in_irq(s_chan_t *chan, const void *in_object) {
    uint16_t end;
    if (chan->available_count >= chan->max_count) {
        return -1;  /* chan buffer overflow */
    }

    end = chan->begin + chan->available_count;
    while (end > chan->max_count)
        end -= chan->max_count;

    memcpy((char*)&chan[1] + end * chan->element_size, in_object, chan->element_size);
    ++chan->available_count;

    s_event_set__in_irq(&chan->event);
    return 0;
}

/*
 * Interrupt reads element from chan
 *  return 0 on chan element was read
 *  return -1 on chan is empty
 */
int s_chan_get__in_irq(s_chan_t *chan, void *out_object) {
    if (chan->available_count <= 0) {
        return -1;  /* chan buffer is empty */
    }

    memcpy(out_object, (char*)&chan[1] + chan->begin * chan->element_size, chan->element_size);

    ++chan->begin;
    while (chan->begin > chan->max_count)
        chan->begin -= chan->max_count;
    --chan->available_count;

    s_event_set__in_irq(&chan->event);
    return 0;
}

#endif
