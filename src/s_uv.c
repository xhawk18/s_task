#include "s_task.h"
#include "s_uv.h"

typedef struct {
    s_event_t event;
    bool trigged;
} s_uv_close_arg_t;

static void s_uv_close_cb(uv_handle_t* handle, void *arg) {
    s_uv_close_arg_t* arg_ = (s_uv_close_arg_t*)arg;
    arg_->trigged = true;
    s_event_set(&arg_->event);
}

void s_uv_close(__async__, uv_handle_t* handle) {
    s_uv_close_arg_t arg;
    arg.trigged = false;
    s_event_init(&arg.event);

    uv_close(handle, s_uv_close_cb, &arg);
    while(!arg.trigged)
        s_event_wait(__await__, &arg.event);
}


typedef struct {
    s_event_t event;
    void* buf;
    size_t buf_len;
    size_t nread;
    bool trigged;
} s_uv_read_start_arg_t;

static void s_uv_read_start_alloc_cb(uv_handle_t* handle,
    size_t suggested_size,
    uv_buf_t* buf,
    void* arg) {
    s_uv_read_start_arg_t* arg_ = (s_uv_read_start_arg_t*)arg;
    buf->base = arg_->buf;
    buf->len = arg_->buf_len;
}

static void s_uv_read_start_read_cb(uv_stream_t* stream,
    ssize_t nread,
    const uv_buf_t* buf,
    void* arg) {
    s_uv_read_start_arg_t* arg_ = (s_uv_read_start_arg_t*)arg;
    arg_->nread = nread;
    arg_->trigged = true;
    s_event_set(&arg_->event);
}

int s_uv_read(__async__, uv_stream_t* handle, void* buf, size_t buf_len, size_t *nread) {
    int ret;
    s_uv_read_start_arg_t arg;
    arg.buf = buf;
    arg.buf_len = buf_len;
    arg.trigged = false;
    s_event_init(&arg.event);

    ret = uv_read_start(handle, s_uv_read_start_alloc_cb, s_uv_read_start_read_cb, &arg);
    if (ret != 0)
        return ret;
    while (!arg.trigged)
        s_event_wait(__await__, &arg.event);

    if (nread != NULL)
        *nread = arg.nread;

    return ret;
}



typedef struct {
    s_event_t event;
    uv_write_t req;
    int status;
    bool trigged;
} s_uv_write_start_arg_t;

static void s_uv_write_cb(uv_write_t* req, int status) {
    s_uv_write_start_arg_t* arg_ = GET_PARENT_ADDR(req, s_uv_write_start_arg_t, req);
    //printf("status = %d\n", status);

    arg_->status = status;
    arg_->trigged = true;
    s_event_set(&arg_->event);
}

int s_uv_write_n(__async__, uv_stream_t* handle, const uv_buf_t bufs[], unsigned int nbufs) {
    int ret;
    s_uv_write_start_arg_t arg;
    arg.trigged = false;
    s_event_init(&arg.event);

    ret = uv_write(&arg.req, handle, bufs, nbufs, s_uv_write_cb);
    if (ret != 0)
        return ret;

    while (!arg.trigged)
        s_event_wait(__await__, &arg.event);
    ret = arg.status;

    return ret;
}

int s_uv_write(__async__, uv_stream_t* handle, const void* buf, size_t buf_len) {
    uv_buf_t uv_buf = uv_buf_init((char *)buf, buf_len);
    return s_uv_write_n(__await__, handle, &uv_buf, 1);
}