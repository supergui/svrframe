#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "nasio.h"

void echo_on_connect(void *conn);
void echo_on_close(void *conn);
void echo_on_message(void *conn, nasio_msg_t *msg);

int main(int argc, char* argv[])
{
	int rv = 0;
	void *env = nasio_env_create( 1000 );
	if( !env )
	{
		printf("nasio env create\n");
		return 1;
	}
	nasio_conn_event_handler_t *handler = (nasio_conn_event_handler_t *)malloc( sizeof(nasio_conn_event_handler_t) ); 
	handler->on_connect = echo_on_connect;
	handler->on_close = echo_on_close;
	handler->on_message = echo_on_message;

	rv = nasio_bind(env, "*", 12388, handler);
	if( rv!=0 )
	{
		printf("add listener fail\n");
		return 2;
	}
	nasio_loop(env, 0);//loop forever

	return 0;
}
void echo_on_connect(void *conn)
{
	printf("echo server comes a connection\n");

}
void echo_on_close(void *conn)
{
	printf("echo server close a connection\n");
}
void echo_on_message(void *conn, nasio_msg_t *msg)
{
    printf("[%u][ %s ]\n", nasio_msg_size(msg), nasio_msg_data(msg));

    char buf[] = "world";
    nasio_msg_t resp;
    nasio_msg_init_size( &resp, sizeof(buf) );
    char *data = nasio_msg_data( &resp );
    memcpy(data, buf, sizeof(buf));
    nasio_send_msg(conn, &resp);
}
