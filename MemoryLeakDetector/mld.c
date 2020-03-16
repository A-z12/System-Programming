/*
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~***************************~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	

	DESCRIPTION - THIS FILE IMPLEMENTS FUNCTIONS USED IN MLD LIBRARY
	
	+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	|	REVISION NUMBER		|	AUTHOR		|	DATE OF REVISION	|	COMMENTS		      |
	-------------------------------------------------------------------------------------------------------------------------------
	|		0		|	AB		|	23-02-2020		|	INITIAL REVISION	      |
	-------------------------------------------------------------------------------------------------------------------------------


	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~***************************~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	

*/

#include "mld.h"
#include <stdio.h>
#include <stdlib.h>

char *DATA_TYPE[] = {"UINT8", "UINT32", "INT32",
		     "CHAR", "OBJ_PTR", "FLOAT",
		     "DOUBLE", "OBJ_STRUCT" };

// FUNCTION TO PRINT DETAILS OF A PARTICULAR STRUCTURE RECORD

void print_structure_rec ( struct_db_rec_t *struct_rec )
{
	int j;

	if ( !struct_rec )
		return;

	field_info_t *field = NULL;


	printf( "%s", struct_rec->struct_name );
	printf( "%d", struct_rec->ds_size );
	printf( "%d\n", struct_rec->n_fields );

	/* HEADING OF DETAILS OF FIELDS */	
	printf( " ------------------------------------------------------------------------------\n");
	printf( " | %-20s | ", "FIELD NAME" );
        printf( "%-10s | ", "DATA TYPE" );
    	printf( "%-10s | ", "OFFSET" );
        printf( "%-25s | \n", "NESTED STRUCTURE NAME" );
	printf( " ------------------------------------------------------------------------------\n");


	/* LOOP TO PRINT DETAILS OF FIELDS
	OF CURRENT STRUCTURE*/
	for( j=0; j< struct_rec->n_fields; j++ )
	{
		field = &struct_rec->fields[j];
		
		printf(" | %-20s | ", field->fname );
		printf("%-10s | ", DATA_TYPE[field->dtype] );
		printf("%-10d | ", field->offset );
		printf("%-25s |\n", field->nested_str_name );
	}
}

/* FUNCTION TO PRINT ALL STRUCTURES AND
   COMPLETE DETAILS OF THEIR RESPECTIVE FIELDS */
void print_structure_db ( struct_db_t * struct_db )
{
	int i=0;
	
	// NULL CHECK
	if ( !struct_db )
		return;

	struct_db_rec_t *struct_rec = struct_db->head;
	
	// PRINTING NO OF STRUCTURES REGISTERED
	printf( "No of Structures Registered = %d\n", struct_db->count);

	while( struct_rec )
	{
		printf( "Structure No : %d (%p)\n", i++, struct_rec );
		print_structure_rec( struct_rec );
		struct_rec = struct_rec->next;
	}
}


/* FUNCTION TO ADD STRUCTURES TO
   STRUCTURE DATABASE */
int add_structure_to_struct_db( struct_db_t *struct_db,
				struct_db_rec_t *struct_rec )
{
	struct_db_rec_t *tmp = struct_db->head;
	
	if( !tmp )
	{
		struct_db->head = struct_rec;
		struct_db->head->next = NULL;
		struct_db->count++;
		return 0;
	}

	struct_rec->next = tmp;
	struct_db->head = struct_rec;
	struct_db->count++;
	return 0;
}
