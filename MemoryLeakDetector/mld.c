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
#include <string.h>

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

/* ----------------------------------------- */

/* PHASE 2 ASSIGNMENT STARTS HERE */

/* FUNCTION TO LOOK UP A PARTICULAR STRUCTURE
   IN STRUCTURE DATABASE */
struct_db_rec_t *struct_db_look_up( struct_db_t *struct_db, char *struct_name )
{

	if ( !struct_db )
		return NULL;
	
	/* POINTING TEMP TO FIRST
	   RECORD OF STRUCTURE DATABASE */
	struct_db_rec_t *temp = struct_db->head;
	
	while ( temp != NULL )
	{
		if ( strcmp( temp->struct_name, struct_name ) == 0 )
			return temp;
		temp = temp->next;
	}
	
	/* RETURN NULL IF STRUCTURE NAME
	   NOT FOUND IN STRUCTURE DATABASE */
	return NULL;
}

/* PHASE 2 ASSIGNMENT ENDS HERE */

/* ----------------------------------------------- */

/* PHASE 2 OBJECT DATABASE
   STARTS HERE */

/* XCALLOC API
   STARTS HERE */

static object_db_rec_t * // why static ?
object_db_look_up( object_db_t *obj_db,
		   void *ptr
		)
{
	object_db_rec_t *tmp = obj_db->head;
	
	
	for(  ; tmp != NULL ; tmp = tmp->next )
	{
		//printf( "\n In object look up function - in loop " );
		if( ptr == tmp->ptr )
			return tmp;
	}
	return NULL;
}

static void // why static ?
add_object_to_object_db( object_db_t *object_db,
			 void *ptr,
			 int units,
			 struct_db_rec_t *struct_rec 
		 	)
{
	/* CHECKING WHETHER THIS OBJECT
	   IS ALREADY PRESENT */
	object_db_rec_t *obj_rec = object_db_look_up( object_db, ptr );
	/* PREVENTING ADDITION OF SAME
	   OBJECT TWICE */
	assert( !obj_rec );

	obj_rec = calloc( 1, sizeof( object_db_rec_t ) );
	
	obj_rec->next = NULL;
	obj_rec->ptr = ptr;
	obj_rec->units = units;
	obj_rec->struct_rec = struct_rec;

	/* ADDING FIRST RECORD IN
	   OBJECT DATABASE */
	if ( !object_db->head ) 
	{
		object_db->head = obj_rec;
		object_db->count = 1;
	}

	/* ADDING MORE RECORDS IN
	   OBJECT DATABASE */
	else
	{
		obj_rec->next = object_db->head;
		object_db->head = obj_rec;
		object_db->count++;
	}
} 
	
void *
xcalloc( object_db_t *object_db,
	 char *struct_name,
	 int units
	)
{
	/*CHECKING WHETHER STRUCTURE EXISTS */
	struct_db_rec_t *struct_rec=struct_db_look_up( object_db->struct_db, struct_name );
	assert( struct_rec );

	/* ALLOCATING MEMORY AND ADDING
	   TO OBJECT DATABASE */
	void *ptr = calloc( units, struct_rec->ds_size );
	add_object_to_object_db( object_db, ptr, units, struct_rec );
	return ptr;
}

/* XCALLOC API
   ENDS HERE */

/* DUMPING FUNCTIONS FOR 
   OBJECT DATABASE */

void print_object_rec( object_db_rec_t *obj_rec, int i )
{
	if( !obj_rec )
		return;
	
        printf( " | %-5d | ", i );
        printf( "%-10p | ", obj_rec->next );
        printf( "%-5u | ", obj_rec->units );
        printf( "%-25s | \n", obj_rec->struct_rec->struct_name );
        
	//printf( "%d %p %u %s\n", i, obj_rec->next, obj_rec->units, obj_rec->struct_rec->struct_name );
}

void print_object_db( object_db_t *object_db )
{
	object_db_rec_t *tmp = object_db->head;
	unsigned int i = 0;
	printf( "PRINTING OBJECT DATABASE\n" );
	
	/* HEADING OF DETAILS OF FIELDS */
        printf( " ------------------------------------------------------------------------------\n");
        printf( " | %-5s | ", "SNO" );
        printf( "%-10s | ", "NEXT PTR" );
        printf( "%-5s | ", "UNITS" );
        printf( "%-25s | \n", "STRUCTURE NAME" );
        printf( " ------------------------------------------------------------------------------\n");

	for( ; tmp; tmp = tmp->next )
		print_object_rec( tmp, i++ );
}

/* PHASE 2 OBJECT DATABASE
   ENDS HERE */

/* ASSIGNMENT 3 STARTS HERE */
void mld_dump_object_rec_detail( object_db_rec_t *obj_rec )
{
	int i=0,j=0;

	//CHECKING WHETHER OBJECT RECORD EXISTS 
	assert( obj_rec );

	field_info_t *field_tmp = NULL;

        field_tmp = obj_rec->struct_rec->fields;
         
        for( int j=1; j<=obj_rec->units; j++ )

         { for( int i=0; i<obj_rec->struct_rec->n_fields; i++ )
          { switch( field_tmp[i].dtype )
           {
 case UINT8 : printf( "%u\t", *(int *)( obj_rec->ptr + (j-1)*obj_rec->struct_rec->ds_size + field_tmp[i].offset ) );
                                     break;             
 case UINT32 : printf( "%u\t", *(int *)( obj_rec->ptr + (j-1)*obj_rec->struct_rec->ds_size + field_tmp[i].offset ) );
                                     break;
 case INT32 : printf( "%d\t", *(int *)( obj_rec->ptr + (j-1)*obj_rec->struct_rec->ds_size + field_tmp[i].offset ) );
                                     break;
 case CHAR : printf( "%s\t", (char*)(obj_rec->ptr + (j-1)*obj_rec->struct_rec->ds_size + field_tmp[i].offset) );
                                        break; 
 case OBJ_PTR : printf( "%p\t", (uintptr_t*)*(uintptr_t*)( obj_rec->ptr + (j-1)*obj_rec->struct_rec->ds_size + field_tmp[i].offset ) );
                                        break;
case FLOAT : printf( "%.2f\t",*( float*) ( obj_rec->ptr + (j-1)*obj_rec->struct_rec->ds_size + field_tmp[i].offset ) );
                                        break;
 case DOUBLE : printf( "%.2f\t",*( float*) ( obj_rec->ptr + (j-1)*obj_rec->struct_rec->ds_size + field_tmp[i].offset ) );
                                        break;
 case OBJ_STRUCT : printf( "%p\t", (uintptr_t*)*(uintptr_t*)( obj_rec->ptr + (j-1)*obj_rec->struct_rec->ds_size + field_tmp[i].offset ) );
                                        break;
          }
         } 
         printf( "\n" );
        }       	
}


int delete_object( object_db_t* object_db, object_db_rec_t* obj_rec  )
{
	object_db_rec_t *object_rec_tmp_p = NULL, *object_rec_tmp = NULL;

	/* CHECKING VALIDITY OF obj_rec */
	assert( obj_rec );

	/* VALDITATING WHETHER OBJECT DB IS NOT EMPTY */
	if( !object_db->head )
	{
		printf( "Object database is empty\n" );
		return -1;
	}

	/* IF THERE IS ONLY ONE OBJECT IN OBJECT DB */	
	if ( object_db->head->next == NULL )
	{
		object_rec_tmp = object_db->head;
		object_db->head = NULL;
	}

	else
        {
                object_rec_tmp_p = object_db->head;
                object_rec_tmp = object_db->head->next;

                /* IF FIRST OBJECT MATCHES */
                if( object_rec_tmp_p == obj_rec )
                {
                        object_rec_tmp = object_db->head;
                        object_db->head = object_db->head->next;
                }
                else
                {
                        do
                        {
                                if( object_rec_tmp == obj_rec )
                                {
                                        object_rec_tmp_p->next = object_rec_tmp->next;
                                        break;
                                }

                                object_rec_tmp_p = object_rec_tmp;
                                object_rec_tmp = object_rec_tmp->next;
                        }while( object_rec_tmp );
                }
	}
	
	free( object_rec_tmp->ptr );

	free( object_rec_tmp );

	object_db->count--;

	return 1;
}

int xfree ( void* ptr, object_db_t* object_db )
{
	/* CHECKING VALIDITY OF PTR */
	assert( ptr );

	/* VERIFYING THAT THE OBJECT DB RECORD
	   FOR THIS OBJECT EXISTS IN OBJECT DB */
	
	object_db_rec_t *obj_rec = object_db_look_up( object_db, ptr );
	if( obj_rec  == NULL )
	{
		printf( "This object does not exist in Object database\n" );
		return -1;
	}

	printf( "\(((((((((((	OBJECT FOUND	)))))))))))" );
	return delete_object( object_db, obj_rec );	
}
	
/* ASSIGNMENT 3 ENDS HERE */	
