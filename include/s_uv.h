/* Copyright xhawk, MIT license */

#ifndef INC_S_UV_H_
#define INC_S_UV_H_

#include "uv.h"
#include "s_task.h"

#ifdef __cplusplus
extern "C" {
#endif

void s_uv_close(__async__, uv_handle_t* handle);
int s_uv_read(__async__, uv_stream_t* handle, void* buf, size_t buf_len, size_t* nread);
int s_uv_write_n(__async__, uv_stream_t* handle, const uv_buf_t bufs[], unsigned int nbufs);
int s_uv_write(__async__, uv_stream_t* handle, const void* buf, size_t buf_len);

#ifdef __cplusplus
}
#endif
#endif

