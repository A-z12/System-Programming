#ifndef __MLD__
	#include<assert.h>
#include<stdint.h>

// Structure database definition begins 

#define MAX_STRUCTURE_NAME_SIZE 128 // .....why structure_name_size?
#define MAX_FIELD_NAME_SIZE 128 // .....why field_name_size?

/* Enumeration for data types */
typedef enum
{
	UINT8,
	UINT32,
	INT32,
	CHAR,
	OBJ_PTR,
	FLOAT,
	DOUBLE,
	OBJ_STRUCT
} data_type_t;

#define OFFSETOF( struct_name, fld_name )	\
	 (uintptr_t)&((( struct_name *)0)->fld_name)

#define FIELD_SIZE( struct_name, fld_name )	\
	sizeof(((struct_name *)0)->fld_name)


// Structure to store the information of one field of a C structure

typedef struct _field_info_ {
	char fname [MAX_FIELD_NAME_SIZE]; // Name of the field
	data_type_t dtype; // Datatype of the field
	unsigned int size; //Size of the field
	unsigned int offset; //Offset of the field
	//Below field is meaningful if dtype = OBJ_PTR or OBJ_STRUCT
	char nested_str_name[MAX_STRUCTURE_NAME_SIZE];
} field_info_t;

// Structure to store information of one structure which could have n fields

typedef struct _struct_db_rec_t
{
	struct _struct_db_rec_t *next; //Pointer to next structure in the linked list
	char struct_name[MAX_STRUCTURE_NAME_SIZE]; //key
	unsigned int ds_size; //Size of the structure
	unsigned int n_fields; //No of fields in the structure
	field_info_t *fields; //pointer to the array of fields
}struct_db_rec_t;

// Head of the linked list representing the structure database

typedef struct _struct_db_
{
	struct_db_rec_t *head;
	unsigned int count;
} struct_db_t;

// Structure database definition ends

// Printing functions

void print_structure_rec ( struct_db_rec_t *struct_rec );

void print_structure_db ( struct_db_t *struct_db );

// Function to add structure record in a structure database

int // return 0 on success , -1 on failure for some reason
add_structure_to_struct_db(struct_db_t *struct_db, struct_db_rec_t *struct_rec); 
// Structure Registration helping APIs

#define FIELD_INFO( struct_name, fld_name, dtype, nested_struct_name ) 	\
	{#fld_name, dtype, FIELD_SIZE( struct_name, fld_name ),		\
		OFFSETOF( struct_name, fld_name ), #nested_struct_name }

#define REG_STRUCT( struct_db, st_name, fields_arr )	\
	do	\
	{	\
		struct_db_rec_t *rec = calloc( 1, sizeof(struct_db_rec_t) );	\
		strncpy( rec->struct_name, #st_name, MAX_STRUCTURE_NAME_SIZE );	\
		rec->ds_size = sizeof(st_name);	\
		rec->n_fields = sizeof( fields_arr )/sizeof( field_info_t );	\
			rec->fields = fields_arr;	\
		if( add_structure_to_struct_db( struct_db, rec ) )	\
		{	\
			assert(0);	\
		}	\
	}while(0);	\

/* PHASE 2 ASSIGNMENT STARTS HERE */
struct_db_rec_t *struct_db_look_up( struct_db_t *struct_db, char *struct_name );
/* PHASE 2 ASSIGNMENT ENDS HERE */

/* OBJECT DATABASE STRUCTURE DEFINITION
   STARTS HERE */
 
typedef struct _object_db_rec_ object_db_rec_t;

struct _object_db_rec_
{
	object_db_rec_t *next;
	void *ptr;
	unsigned int units;
	struct_db_rec_t *struct_rec;
};

typedef struct _object_db_
{
	struct_db_t *struct_db;
	object_db_rec_t *head;
	unsigned int count;
}object_db_t;

/* DUMPING FUNCTIONS */
void print_object_rec( object_db_rec_t *obj_rec, int i ); // WHY i ?

void print_object_db( object_db_t *object_db );

/* API TO MALLOC
   THE OBJECT */
void *xcalloc( object_db_t *object_db, char *struct_name, int units );

/* ASSIGNMENT 3 STARTS */
void mld_dump_object_rec_detail( object_db_rec_t *obj_rec );

int delete_object( object_db_t* object_db, object_db_rec_t* obj_rec  );

int xfree ( void* ptr, object_db_t* object_db );

#endif

