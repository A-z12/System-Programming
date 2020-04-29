#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>

#define SOCKET_NAME "/tmp/DemoSocket"
#define BUFFER_SIZE 128

int main()
{
	struct sockaddr_un name;

#if 0
	struct sockaddr_un {
		sa_family_t sun_family;   /* AF_UNIX */
		char sun_path[108];	  /* pathname */
	};
#endif

	int ret;
	int connection_socket;
	int data_socket;
	int result;
	int data;
	char buffer[ BUFFER_SIZE ];

	/* REMOVING THE SOCKET IF EXISTS */
	unlink( SOCKET_NAME );

	/* CREATE MASTER SOCKET */
	/* SOCK_DGRAM FOR DATAGRAM BASED COMMUNICATION */
	connection_socket = socket( AF_UNIX, SOCK_STREAM, 0 );
	
	if( connection_socket == -1 )
	{
		perror( "Master socket creation failed" );
		exit( EXIT_FAILURE );
	}

	printf( "Master socket created\n" );

	/* INITIALIZE */
	memset( &name, 0, sizeof( struct sockaddr_un ) );

		
	/* SPECIFY COCKET CREDENTIALS */
	name.sun_family = AF_UNIX;
	strncpy( name.sun_path, SOCKET_NAME, sizeof( name.sun_path ) -1 );

	/* BIND SOCKET TO SOCKET NAME */
	/* PURPOSE OF bind() SYS CALL IS THAT SERVER IS
	   TELLING THE OS THAT IF THE CLIENT SENDS THE
	   DATA DESTINED TO "\tmp\DemoSocket" THEN OS
	   SHOULD DIVERT THIS TO IT */

	ret = bind( connection_socket, ( const struct sockaddr *) &name, sizeof( struct sockaddr_un ) );

	if( ret == -1 )
	{
		perror( "bind() system call failed" );
		exit( EXIT_FAILURE );
	}

	printf( "bind() system call succeeded\n" );

	/*  WE WILL NOW ASK OS TO SET THE BACKLOG
	   SIZE TO 20, SO THAT AT A MAXIMUM OF 20 REQUESTS
	   CAN WAIT TO GET SERVICE FROM SERVER, OTHER REQUESTS
	   WILL THEN BE DROPPED
	*/

	ret = listen( connection_socket, 20 );

	if( ret == -1 )
	{
		perror( "listen() system call failed" );
		exit( EXIT_FAILURE );
	}

	/* SERVER CONTENT, SERVER USUALLY RUNS 24x7 */

	for( ;; )
	{
		/* ACCEPT() IS BLOCKING SYSTEM CALL */
		data_socket = accept( connection_socket, NULL, NULL );

		if( data_socket == -1 )
		{
			perror( "accept() sys call failed");
			exit( EXIT_FAILURE );
		};

		printf( "Connection accepted from client\n" );

		result = 0;

		for ( ;; )
		{
			memset( buffer, 0, BUFFER_SIZE );

			printf( "Waiting to recieve data from client\n" );
	
			/* READ() IS BLOCKING SYS CALL */
			ret = read( data_socket, buffer, BUFFER_SIZE );

			if( ret == -1 )
			{
				perror( "read" );
				exit( EXIT_FAILURE );
			}

			printf( "Data read successfully from the client\n" );

			/* ADDING RECIEVED DATA TO RESULT*/
			memcpy( &data, buffer ,sizeof(int) );
			
			/* BREAKING FROM LOOP IF CLIENT SENT 0 */
			if( data == 0 )
				break;

			result += data;		
		}
		
		/* SEND RESULT */
		memset( buffer, 0, BUFFER_SIZE );
		sprintf( buffer, "Result - %d", result );

		printf( "Writing result to the client\n" );
		ret = write( data_socket, buffer, BUFFER_SIZE );

		if( ret == -1 )
		{
			perror( "Write() sys call failed" );
			exit( EXIT_FAILURE );
		}

		printf( "Successfully written the result to the client\n" );

		/* CLOSE SOCKET */
		close( data_socket );
	}

	/* CLOSE MASTER SOCKET FILE DESCRIPTOR */
	close( connection_socket );
	printf( "Connection closed\n" );

	/* SERVER SHOULD RELEASE RESOURCES BEFORE TERMINATING */
	unlink( SOCKET_NAME );
	exit( EXIT_SUCCESS );
}




















