/* FILE NAME - app.c */

#include "mld.h"
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>

/* EMPLOYEE STRUCTURE */
typedef struct emp_
{
	char emp_name[30];
	unsigned int emp_id;
	unsigned int age;
	struct emp_ *mgr;
	float salary;
} emp_t;

/* STUDENT STRUCTURE */
typedef struct student_
{
	char stud_name[32];
	unsigned int rollno;
	unsigned int age;
	float aggregate;
	struct student_ *best_colleague;
} student_t;

int main( int argc, char **argv )
{
	/* STEP 1 : INITIALIZE A NEW
		    STRUCTURE DATABASE */
	struct_db_t *struct_db = calloc( 1, sizeof( struct_db_t ) );

	/* STORING INFORMATION OF FIELDS
	    FOR EMPLOYEE STRUCTURE */
	static field_info_t emp_fields[] = {
		FIELD_INFO( emp_t, emp_name, CHAR, 0 ),
		FIELD_INFO( emp_t, emp_id, UINT32,0 ),
		FIELD_INFO( emp_t, age, UINT32, 0 ),
		FIELD_INFO( emp_t, mgr, OBJ_PTR, emp_t ),
		FIELD_INFO( emp_t, salary, FLOAT, 0 )
	};


	/* REGISTERING EMPLOYEE STRUCTURE
	   IN STRUCTURE DATABASE */
	REG_STRUCT( struct_db, emp_t, emp_fields );

	/* STORING INFORMATION OF FIELDS
	   OF STUDENT STRUCTURE */
	static field_info_t stud_fields[] = {
		FIELD_INFO( student_t, stud_name, CHAR, 0 ),
		FIELD_INFO( student_t, rollno, UINT32, 0 ),
		FIELD_INFO( student_t, age, UINT32, 0 ),
		FIELD_INFO( student_t, aggregate, FLOAT, 0),
		FIELD_INFO( student_t, best_colleague, OBJ_PTR, student_t)
	};

	/* REGISTERING STUDENT STRUCTURE
	   IN STRUCTURE DATABASE */
	REG_STRUCT( struct_db, student_t, stud_fields );

	/* PRINTING COMPLETE STRUCTURE DATABASE */
	print_structure_db( struct_db );

	/* PHASE 2 ASSIGNMENT STARTS HERE */
	printf( "\n PHASE 2 ASSIGNMNET STARTS HERE \n");

	struct_db_rec_t *k =  struct_db_look_up ( struct_db, "emp_t" ) ;
	
	printf("%d\n", k->n_fields );
	/* PHASE 2 ASSIGNMENT ENDS HERE */


	/* PHASE 2 OBJECT DATABASE
	   STARTS HERE */
	/* STEP 1 : INITIALIZE A NEW
	   OBJECT DATABASE */
	object_db_t *object_db = calloc( 1, sizeof( object_db_t ) );	    object_db->struct_db = struct_db;
	
	/* STEP 2 : CREATE SOME SAMPLE OBJECTS */
	student_t *david = xcalloc( object_db, "student_t", 1 );
	student_t *abraham = xcalloc( object_db, "student_t", 1 );
	emp_t *joseph = xcalloc( object_db, "emp_t", 2 );
	/* STEP 3 : PRINTING OBJECT DATABASE */
	print_object_db( object_db );
	
	return(0);
}
