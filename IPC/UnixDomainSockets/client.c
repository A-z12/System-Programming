#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_NAME "/tmp/DemoSocket"
#define BUFFER_SIZE 128

int main()
{
	struct sockaddr_un addr;
	int data_socket;
	char buffer[ BUFFER_SIZE ];
	int ret;
	int i;

	/* CREATE DATA SOCKET */
	data_socket = socket( AF_UNIX, SOCK_STREAM, 0 );

	if( data_socket == -1 )
	{
		perror( "Failed to create data socket");
		exit( EXIT_FAILURE );
	}

	memset( &addr, 0 ,sizeof(struct sockaddr_un ) );

	addr.sun_family = AF_UNIX;
	strncpy( addr.sun_path, SOCKET_NAME, sizeof( addr.sun_path ) -1 );

	/* CONNECT  TO THE SOCKET ADDRS */

	ret = connect( data_socket, (const struct sockaddr * ) &addr, sizeof( struct sockaddr_un ) );

	if ( ret == -1 )
	{
		perror( "Connection to the socket adrs failed" );
		exit( EXIT_FAILURE );
	}

	printf( "Connected to the socket with the given socket adrs adrs successfully" );

	do
	{
		printf( "Enter the number to send to server : ");
		scanf( "%d", &i );
		
		ret = write( data_socket, &i, sizeof( int ) );

		if( ret == -1 )
		{
			perror( "failed to write data to server" );
			break;
		}
		
		printf( "No of bytes sent = %d, data sent = %d\n", ret, i );

	} while( i );

	/* REQUEST RESULT */

	memset( buffer, 0, BUFFER_SIZE );
	ret = read( data_socket, buffer, BUFFER_SIZE );
	
	if( ret == -1 )
	{
		perror( "Could not read the result" );
		exit( EXIT_FAILURE );
	}
	
	/* ENSURE BUFFER IS 0-TERMINATED */
	printf( "Result = %s\n", buffer );

	/* CLOSE SOCKET */
	close( data_socket );

	exit( EXIT_SUCCESS );

}































