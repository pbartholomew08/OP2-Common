//
// auto-generated by op_seq_gen.m on 24-Oct-2011 15:36:23
//

//
// header
//

#include "op_lib_core.h"

void op_arg_set(int n, op_arg arg, char **p_arg){
  int n2;
  if (arg.map==NULL)         // identity mapping, or global data
    n2 = n;
  else                       // standard pointers
    n2 = arg.map->map[arg.idx+n*arg.map->dim];

  *p_arg = arg.data + n2*arg.size;
}


//
// op_par_loop routine for 1 arguments
//

template < class T0 >
void op_par_loop(void (*kernel)( T0* ),
  char const * name, op_set set,
  op_arg arg0 ) {

  char *p_arg0;

  // consistency checks

  int ninds=0;

  if (OP_diags>0) {
   op_arg_check(set,0 ,arg0 ,&ninds,name);
  }

  if (OP_diags>2) {
    if (ninds==0)
      printf(" kernel routine w/o indirection:  %s \n",name);
    else
      printf(" kernel routine with indirection: %s \n",name);
  }

  // loop over set elements

  for (int n=0; n<set->size; n++) {
    op_arg_set(n,arg0 ,&p_arg0 );

    // call kernel function, passing in pointers to data

    kernel( (T0 *)p_arg0 );
  }
}

//
// op_par_loop routine for 2 arguments
//

template < class T0, class T1 >
void op_par_loop(void (*kernel)( T0*, T1* ),
  char const * name, op_set set,
  op_arg arg0, op_arg arg1 ) {

  char *p_arg0, *p_arg1;

  // consistency checks

  int ninds=0;

  if (OP_diags>0) {
   op_arg_check(set,0 ,arg0 ,&ninds,name);
   op_arg_check(set,1 ,arg1 ,&ninds,name);
  }

  if (OP_diags>2) {
    if (ninds==0)
      printf(" kernel routine w/o indirection:  %s \n",name);
    else
      printf(" kernel routine with indirection: %s \n",name);
  }

  // loop over set elements

  for (int n=0; n<set->size; n++) {
    op_arg_set(n,arg0 ,&p_arg0 );
    op_arg_set(n,arg1 ,&p_arg1 );

    // call kernel function, passing in pointers to data

    kernel( (T0 *)p_arg0,  (T1 *)p_arg1 );
  }
}

//
// op_par_loop routine for 3 arguments
//

template < class T0, class T1, class T2 >
void op_par_loop(void (*kernel)( T0*, T1*, T2* ),
  char const * name, op_set set,
  op_arg arg0, op_arg arg1, op_arg arg2 ) {

  char *p_arg0, *p_arg1, *p_arg2;

  // consistency checks

  int ninds=0;

  if (OP_diags>0) {
   op_arg_check(set,0 ,arg0 ,&ninds,name);
   op_arg_check(set,1 ,arg1 ,&ninds,name);
   op_arg_check(set,2 ,arg2 ,&ninds,name);
  }

  if (OP_diags>2) {
    if (ninds==0)
      printf(" kernel routine w/o indirection:  %s \n",name);
    else
      printf(" kernel routine with indirection: %s \n",name);
  }

  // loop over set elements

  for (int n=0; n<set->size; n++) {
    op_arg_set(n,arg0 ,&p_arg0 );
    op_arg_set(n,arg1 ,&p_arg1 );
    op_arg_set(n,arg2 ,&p_arg2 );

    // call kernel function, passing in pointers to data

    kernel( (T0 *)p_arg0,  (T1 *)p_arg1,  (T2 *)p_arg2 );
  }
}

//
// op_par_loop routine for 4 arguments
//

template < class T0, class T1, class T2, class T3 >
void op_par_loop(void (*kernel)( T0*, T1*, T2*, T3* ),
  char const * name, op_set set,
  op_arg arg0, op_arg arg1, op_arg arg2, op_arg arg3 ) {

  char *p_arg0, *p_arg1, *p_arg2, *p_arg3;

  // consistency checks

  int ninds=0;

  if (OP_diags>0) {
   op_arg_check(set,0 ,arg0 ,&ninds,name);
   op_arg_check(set,1 ,arg1 ,&ninds,name);
   op_arg_check(set,2 ,arg2 ,&ninds,name);
   op_arg_check(set,3 ,arg3 ,&ninds,name);
  }

  if (OP_diags>2) {
    if (ninds==0)
      printf(" kernel routine w/o indirection:  %s \n",name);
    else
      printf(" kernel routine with indirection: %s \n",name);
  }

  // loop over set elements

  for (int n=0; n<set->size; n++) {
    op_arg_set(n,arg0 ,&p_arg0 );
    op_arg_set(n,arg1 ,&p_arg1 );
    op_arg_set(n,arg2 ,&p_arg2 );
    op_arg_set(n,arg3 ,&p_arg3 );

    // call kernel function, passing in pointers to data

    kernel( (T0 *)p_arg0,  (T1 *)p_arg1,  (T2 *)p_arg2,  (T3 *)p_arg3 );
  }
}

//
// op_par_loop routine for 5 arguments
//

template < class T0, class T1, class T2, class T3,
           class T4 >
void op_par_loop(void (*kernel)( T0*, T1*, T2*, T3*,
                                 T4* ),
  char const * name, op_set set,
  op_arg arg0, op_arg arg1, op_arg arg2, op_arg arg3,
  op_arg arg4 ) {

  char *p_arg0, *p_arg1, *p_arg2, *p_arg3,
       *p_arg4;

  // consistency checks

  int ninds=0;

  if (OP_diags>0) {
   op_arg_check(set,0 ,arg0 ,&ninds,name);
   op_arg_check(set,1 ,arg1 ,&ninds,name);
   op_arg_check(set,2 ,arg2 ,&ninds,name);
   op_arg_check(set,3 ,arg3 ,&ninds,name);
   op_arg_check(set,4 ,arg4 ,&ninds,name);
  }

  if (OP_diags>2) {
    if (ninds==0)
      printf(" kernel routine w/o indirection:  %s \n",name);
    else
      printf(" kernel routine with indirection: %s \n",name);
  }

  // loop over set elements

  for (int n=0; n<set->size; n++) {
    op_arg_set(n,arg0 ,&p_arg0 );
    op_arg_set(n,arg1 ,&p_arg1 );
    op_arg_set(n,arg2 ,&p_arg2 );
    op_arg_set(n,arg3 ,&p_arg3 );
    op_arg_set(n,arg4 ,&p_arg4 );

    // call kernel function, passing in pointers to data

    kernel( (T0 *)p_arg0,  (T1 *)p_arg1,  (T2 *)p_arg2,  (T3 *)p_arg3,
            (T4 *)p_arg4 );
  }
}

//
// op_par_loop routine for 6 arguments
//

template < class T0, class T1, class T2, class T3,
           class T4, class T5 >
void op_par_loop(void (*kernel)( T0*, T1*, T2*, T3*,
                                 T4*, T5* ),
  char const * name, op_set set,
  op_arg arg0, op_arg arg1, op_arg arg2, op_arg arg3,
  op_arg arg4, op_arg arg5 ) {

  char *p_arg0, *p_arg1, *p_arg2, *p_arg3,
       *p_arg4, *p_arg5;

  // consistency checks

  int ninds=0;

  if (OP_diags>0) {
   op_arg_check(set,0 ,arg0 ,&ninds,name);
   op_arg_check(set,1 ,arg1 ,&ninds,name);
   op_arg_check(set,2 ,arg2 ,&ninds,name);
   op_arg_check(set,3 ,arg3 ,&ninds,name);
   op_arg_check(set,4 ,arg4 ,&ninds,name);
   op_arg_check(set,5 ,arg5 ,&ninds,name);
  }

  if (OP_diags>2) {
    if (ninds==0)
      printf(" kernel routine w/o indirection:  %s \n",name);
    else
      printf(" kernel routine with indirection: %s \n",name);
  }

  // loop over set elements

  for (int n=0; n<set->size; n++) {
    op_arg_set(n,arg0 ,&p_arg0 );
    op_arg_set(n,arg1 ,&p_arg1 );
    op_arg_set(n,arg2 ,&p_arg2 );
    op_arg_set(n,arg3 ,&p_arg3 );
    op_arg_set(n,arg4 ,&p_arg4 );
    op_arg_set(n,arg5 ,&p_arg5 );

    // call kernel function, passing in pointers to data

    kernel( (T0 *)p_arg0,  (T1 *)p_arg1,  (T2 *)p_arg2,  (T3 *)p_arg3,
            (T4 *)p_arg4,  (T5 *)p_arg5 );
  }
}

//
// op_par_loop routine for 7 arguments
//

template < class T0, class T1, class T2, class T3,
           class T4, class T5, class T6 >
void op_par_loop(void (*kernel)( T0*, T1*, T2*, T3*,
                                 T4*, T5*, T6* ),
  char const * name, op_set set,
  op_arg arg0, op_arg arg1, op_arg arg2, op_arg arg3,
  op_arg arg4, op_arg arg5, op_arg arg6 ) {

  char *p_arg0, *p_arg1, *p_arg2, *p_arg3,
       *p_arg4, *p_arg5, *p_arg6;

  // consistency checks

  int ninds=0;

  if (OP_diags>0) {
   op_arg_check(set,0 ,arg0 ,&ninds,name);
   op_arg_check(set,1 ,arg1 ,&ninds,name);
   op_arg_check(set,2 ,arg2 ,&ninds,name);
   op_arg_check(set,3 ,arg3 ,&ninds,name);
   op_arg_check(set,4 ,arg4 ,&ninds,name);
   op_arg_check(set,5 ,arg5 ,&ninds,name);
   op_arg_check(set,6 ,arg6 ,&ninds,name);
  }

  if (OP_diags>2) {
    if (ninds==0)
      printf(" kernel routine w/o indirection:  %s \n",name);
    else
      printf(" kernel routine with indirection: %s \n",name);
  }

  // loop over set elements

  for (int n=0; n<set->size; n++) {
    op_arg_set(n,arg0 ,&p_arg0 );
    op_arg_set(n,arg1 ,&p_arg1 );
    op_arg_set(n,arg2 ,&p_arg2 );
    op_arg_set(n,arg3 ,&p_arg3 );
    op_arg_set(n,arg4 ,&p_arg4 );
    op_arg_set(n,arg5 ,&p_arg5 );
    op_arg_set(n,arg6 ,&p_arg6 );

    // call kernel function, passing in pointers to data

    kernel( (T0 *)p_arg0,  (T1 *)p_arg1,  (T2 *)p_arg2,  (T3 *)p_arg3,
            (T4 *)p_arg4,  (T5 *)p_arg5,  (T6 *)p_arg6 );
  }
}

//
// op_par_loop routine for 8 arguments
//

template < class T0, class T1, class T2, class T3,
           class T4, class T5, class T6, class T7 >
void op_par_loop(void (*kernel)( T0*, T1*, T2*, T3*,
                                 T4*, T5*, T6*, T7* ),
  char const * name, op_set set,
  op_arg arg0, op_arg arg1, op_arg arg2, op_arg arg3,
  op_arg arg4, op_arg arg5, op_arg arg6, op_arg arg7 ) {

  char *p_arg0, *p_arg1, *p_arg2, *p_arg3,
       *p_arg4, *p_arg5, *p_arg6, *p_arg7;

  // consistency checks

  int ninds=0;

  if (OP_diags>0) {
   op_arg_check(set,0 ,arg0 ,&ninds,name);
   op_arg_check(set,1 ,arg1 ,&ninds,name);
   op_arg_check(set,2 ,arg2 ,&ninds,name);
   op_arg_check(set,3 ,arg3 ,&ninds,name);
   op_arg_check(set,4 ,arg4 ,&ninds,name);
   op_arg_check(set,5 ,arg5 ,&ninds,name);
   op_arg_check(set,6 ,arg6 ,&ninds,name);
   op_arg_check(set,7 ,arg7 ,&ninds,name);
  }

  if (OP_diags>2) {
    if (ninds==0)
      printf(" kernel routine w/o indirection:  %s \n",name);
    else
      printf(" kernel routine with indirection: %s \n",name);
  }

  // loop over set elements

  for (int n=0; n<set->size; n++) {
    op_arg_set(n,arg0 ,&p_arg0 );
    op_arg_set(n,arg1 ,&p_arg1 );
    op_arg_set(n,arg2 ,&p_arg2 );
    op_arg_set(n,arg3 ,&p_arg3 );
    op_arg_set(n,arg4 ,&p_arg4 );
    op_arg_set(n,arg5 ,&p_arg5 );
    op_arg_set(n,arg6 ,&p_arg6 );
    op_arg_set(n,arg7 ,&p_arg7 );

    // call kernel function, passing in pointers to data

    kernel( (T0 *)p_arg0,  (T1 *)p_arg1,  (T2 *)p_arg2,  (T3 *)p_arg3,
            (T4 *)p_arg4,  (T5 *)p_arg5,  (T6 *)p_arg6,  (T7 *)p_arg7 );
  }
}

//
// op_par_loop routine for 9 arguments
//

template < class T0, class T1, class T2, class T3,
           class T4, class T5, class T6, class T7,
           class T8 >
void op_par_loop(void (*kernel)( T0*, T1*, T2*, T3*,
                                 T4*, T5*, T6*, T7*,
                                 T8* ),
  char const * name, op_set set,
  op_arg arg0, op_arg arg1, op_arg arg2, op_arg arg3,
  op_arg arg4, op_arg arg5, op_arg arg6, op_arg arg7,
  op_arg arg8 ) {

  char *p_arg0, *p_arg1, *p_arg2, *p_arg3,
       *p_arg4, *p_arg5, *p_arg6, *p_arg7,
       *p_arg8;

  // consistency checks

  int ninds=0;

  if (OP_diags>0) {
   op_arg_check(set,0 ,arg0 ,&ninds,name);
   op_arg_check(set,1 ,arg1 ,&ninds,name);
   op_arg_check(set,2 ,arg2 ,&ninds,name);
   op_arg_check(set,3 ,arg3 ,&ninds,name);
   op_arg_check(set,4 ,arg4 ,&ninds,name);
   op_arg_check(set,5 ,arg5 ,&ninds,name);
   op_arg_check(set,6 ,arg6 ,&ninds,name);
   op_arg_check(set,7 ,arg7 ,&ninds,name);
   op_arg_check(set,8 ,arg8 ,&ninds,name);
  }

  if (OP_diags>2) {
    if (ninds==0)
      printf(" kernel routine w/o indirection:  %s \n",name);
    else
      printf(" kernel routine with indirection: %s \n",name);
  }

  // loop over set elements

  for (int n=0; n<set->size; n++) {
    op_arg_set(n,arg0 ,&p_arg0 );
    op_arg_set(n,arg1 ,&p_arg1 );
    op_arg_set(n,arg2 ,&p_arg2 );
    op_arg_set(n,arg3 ,&p_arg3 );
    op_arg_set(n,arg4 ,&p_arg4 );
    op_arg_set(n,arg5 ,&p_arg5 );
    op_arg_set(n,arg6 ,&p_arg6 );
    op_arg_set(n,arg7 ,&p_arg7 );
    op_arg_set(n,arg8 ,&p_arg8 );

    // call kernel function, passing in pointers to data

    kernel( (T0 *)p_arg0,  (T1 *)p_arg1,  (T2 *)p_arg2,  (T3 *)p_arg3,
            (T4 *)p_arg4,  (T5 *)p_arg5,  (T6 *)p_arg6,  (T7 *)p_arg7,
            (T8 *)p_arg8 );
  }
}

//
// op_par_loop routine for 10 arguments
//

template < class T0, class T1, class T2, class T3,
           class T4, class T5, class T6, class T7,
           class T8, class T9 >
void op_par_loop(void (*kernel)( T0*, T1*, T2*, T3*,
                                 T4*, T5*, T6*, T7*,
                                 T8*, T9* ),
  char const * name, op_set set,
  op_arg arg0, op_arg arg1, op_arg arg2, op_arg arg3,
  op_arg arg4, op_arg arg5, op_arg arg6, op_arg arg7,
  op_arg arg8, op_arg arg9 ) {

  char *p_arg0, *p_arg1, *p_arg2, *p_arg3,
       *p_arg4, *p_arg5, *p_arg6, *p_arg7,
       *p_arg8, *p_arg9;

  // consistency checks

  int ninds=0;

  if (OP_diags>0) {
   op_arg_check(set,0 ,arg0 ,&ninds,name);
   op_arg_check(set,1 ,arg1 ,&ninds,name);
   op_arg_check(set,2 ,arg2 ,&ninds,name);
   op_arg_check(set,3 ,arg3 ,&ninds,name);
   op_arg_check(set,4 ,arg4 ,&ninds,name);
   op_arg_check(set,5 ,arg5 ,&ninds,name);
   op_arg_check(set,6 ,arg6 ,&ninds,name);
   op_arg_check(set,7 ,arg7 ,&ninds,name);
   op_arg_check(set,8 ,arg8 ,&ninds,name);
   op_arg_check(set,9 ,arg9 ,&ninds,name);
  }

  if (OP_diags>2) {
    if (ninds==0)
      printf(" kernel routine w/o indirection:  %s \n",name);
    else
      printf(" kernel routine with indirection: %s \n",name);
  }

  // loop over set elements

  for (int n=0; n<set->size; n++) {
    op_arg_set(n,arg0 ,&p_arg0 );
    op_arg_set(n,arg1 ,&p_arg1 );
    op_arg_set(n,arg2 ,&p_arg2 );
    op_arg_set(n,arg3 ,&p_arg3 );
    op_arg_set(n,arg4 ,&p_arg4 );
    op_arg_set(n,arg5 ,&p_arg5 );
    op_arg_set(n,arg6 ,&p_arg6 );
    op_arg_set(n,arg7 ,&p_arg7 );
    op_arg_set(n,arg8 ,&p_arg8 );
    op_arg_set(n,arg9 ,&p_arg9 );

    // call kernel function, passing in pointers to data

    kernel( (T0 *)p_arg0,  (T1 *)p_arg1,  (T2 *)p_arg2,  (T3 *)p_arg3,
            (T4 *)p_arg4,  (T5 *)p_arg5,  (T6 *)p_arg6,  (T7 *)p_arg7,
            (T8 *)p_arg8,  (T9 *)p_arg9 );
  }
}

