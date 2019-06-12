#ifndef BUFFER_H
#define BUFFER_H

#include "ap_int.h"

template <typename T, int LROW, int LCOL>
  class d_linebuffer{
 public:
  T M[LROW][LCOL];

  d_linebuffer(){
#pragma AP ARRAY_PARTITION variable=M dim=1 complete
  };
  ~d_linebuffer(){};
  void shift_up(int col);
  void shift_down(int col);
  void insert_top(T value, int col);
  void insert_bottom(T value, int col);
  void print(int StartCol, int EndCol);
  T getval(int RowIndex,int ColIndex);
};

/* Line buffer print function.
 * Prints the values of all rows in the line buffer
 * between StartCol and EndCol
 */
template <typename T, int LROW, int LCOL>
  void d_linebuffer<T,LROW,LCOL>::print(int StartCol, int EndCol)
{
  int i, j;
  for(i = LROW-1; i > -1; i--){
    printf("Line %d:\t",i);
    for(j=StartCol; j < EndCol; j++){
      printf("%d\t",M[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}

/* Line buffer shift up
 * Assumes new data pixel will be entered at the bottom of the line buffer
 * The bottom is row = 0
 */
template <typename T, int LROW, int LCOL>
  void d_linebuffer<T,LROW,LCOL>::shift_up(int col)
{
#pragma AP inline
  int i;
  for(i = LROW-1; i > 0; i--){
#pragma AP unroll
    M[i][col] = M[i-1][col];
  }
}

/* Line buffer shift down
 * Assumes new data pixel will be entered at the top of the line buffer
 * The bottom is row = LROW - 1
 */
template <typename T, int LROW, int LCOL>
  void d_linebuffer<T,LROW,LCOL>::shift_down(int col)
{
#pragma AP inline
  int i;
  for(i = 0; i < LROW-1; i++){
#pragma AP unroll
    M[i][col] = M[i+1][col];
  }
}

/* Line buffer insert bottom
 * Inserts a new value in the bottom row of the line buffer at column = col
 * The bottom is row = 0
 */
template <typename T, int LROW, int LCOL>
  void d_linebuffer<T,LROW,LCOL>::insert_bottom(T value, int col)
{
#pragma AP inline

  M[0][col] = value;
}

/* Line buffer insert top
 * Inserts a new value in the top row of the line buffer at column = col
 * The bottom is row = LROW - 1
 */
template <typename T, int LROW, int LCOL>
  void d_linebuffer<T,LROW,LCOL>::insert_top(T value, int col)
{
#pragma AP inline

  M[LROW-1][col] = value;
}

/* Line buffer getval
 * Returns the data value in the line buffer at position RowIndex, ColIndex
 */
template <typename T, int LROW, int LCOL>
  T d_linebuffer<T,LROW,LCOL>::getval(int RowIndex,int ColIndex)
{
#pragma AP inline

  T return_value;
  return_value = M[RowIndex][ColIndex];
  return return_value;
}

/* Memory window class definition */
template <typename T, int LROW, int LCOL>
  class d_window{
 public:
  T M[LROW][LCOL];

  d_window(){
#pragma AP ARRAY_PARTITION variable=M dim=0 complete
  };
  ~d_window(){};
  void shift_right();
  void shift_left();
  void shift_up();
  void shift_down();
  void insert(T value, int row,int col);
  void print();
  T getval(int RowIndex,int ColIndex);
};

/* Window print
 * Prints the entire contents of the memory window
 */
template <typename T, int LROW, int LCOL>
  void d_window<T,LROW,LCOL>::print()
{
  int i, j;
  printf("Window Size = %d x %d\n",LROW,LCOL);
  printf("Col \t");
  for(j = 0; j < LCOL; j++){
    printf("%d \t",j);
  }
  printf("\n");
  for(i = 0; i < LROW; i++){
    printf("Row %d: \t",i);
    for(j=0; j < LCOL; j++){
      printf("%d\t",M[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}

/* Window shift right
 * Moves all the contents of the window horizontally
 * Assumes new values will be placed in column = LCOL-1
 */
template <typename T, int LROW, int LCOL>
  void d_window<T,LROW,LCOL>::shift_right()
{
#pragma AP inline
  int i, j;
  for(i = 0; i < LROW; i++){
#pragma AP unroll
    for(j=0; j < LCOL-1; j++){
#pragma AP unroll
      M[i][j] = M[i][j+1];
    }
  }
}

/* Window shift left
 * Moves all the contents of the window horizontally
 * Assumes new values will be placed in column = 0
 */
template <typename T, int LROW, int LCOL>
  void d_window<T,LROW,LCOL>::shift_left()
{
#pragma AP inline
  int i, j;
  for(i = 0; i < LROW; i++){
#pragma AP unroll
    for(j=LCOL-1; j > 0; j--){
#pragma AP unroll
      M[i][j] = M[i][j-1];
    }
  }
}

/* Window shift up
 * Moves all the contents of the window vertically
 * Assumes new values will be placed in row = 0
 */
template <typename T, int LROW, int LCOL>
  void d_window<T,LROW,LCOL>::shift_up()
{
#pragma AP inline
  int i, j;
  for(i = LROW-1; i > 0; i--){
#pragma AP unroll
    for(j=0; j < LCOL; j++){
#pragma AP unroll
      M[i][j] = M[i-1][j];
    }
  }
}

/* Window shift down
 * Moves all the contents of the window vertically
 * Assumes new values will be placed in row = LROW - 1
 */
template <typename T, int LROW, int LCOL>
  void d_window<T,LROW,LCOL>::shift_down()
{
#pragma AP inline
  int i, j;
  for(i = 0; i < LROW-1; i++){
#pragma AP unroll
    for(j=0; j < LCOL; j++){
#pragma AP unroll
      M[i][j] = M[i+1][j];
    }
  }
}

/* Window insert
 * Inserts a new value at any location of the window
 */
template <typename T, int LROW, int LCOL>
  void d_window<T,LROW,LCOL>::insert(T value, int row, int col)
{
#pragma AP inline
  M[row][col] = value;
}

/* Window getval
 * Returns the value of any window location
 */
template <typename T, int LROW, int LCOL>
  T d_window<T,LROW,LCOL>::getval(int RowIndex, int ColIndex)
{
#pragma AP inline
  T return_value;
  return_value = M[RowIndex][ColIndex];
  return return_value;
}



/* Line buffer class definition */
template <typename T, int LDEP, int LROW, int LCOL>
  class t_linebuffer{
 public:
  T M[LDEP][LROW][LCOL];

  t_linebuffer(){
#pragma AP ARRAY_PARTITION variable=M dim=2 complete
  };
  ~t_linebuffer(){};
  void shift_up(int dep,int col);
  void shift_down(int dep,int col);
  void insert_top(T value, int dep, int col);
  void insert_bottom(T value, int dep, int col);
  void print(int StartCol, int EndCol);
  T getval(int DepIdex, int RowIndex,int ColIndex);};

/* Line buffer print function.
 * Prints the values of all rows in the line buffer
 * between StartCol and EndCol
 */
template <typename T, int LDEP, int LROW, int LCOL>
  void t_linebuffer<T,LDEP,LROW,LCOL>::print(int StartCol, int EndCol)
{
  int i, j, k;
  for(k = 0; k < LDEP; k++){
	  for(i = LROW-1; i > -1; i--){
		printf("Line %d:\t",i);
		for(j=StartCol; j < EndCol; j++){
		  printf("%d\t",M[k][i][j]);
		}
		printf("\n");
	  }
	  printf("\n");
  }
  printf("\n");
}

/* Line buffer shift up
 * Assumes new data pixel will be entered at the bottom of the line buffer
 * The bottom is row = 0
 */
template <typename T, int LDEP, int LROW, int LCOL>
  void t_linebuffer<T,LDEP,LROW,LCOL>::shift_up(int dep,int col)
{
#pragma AP inline
  int i;
	  for(i = 0; i < LROW-1; i++){
	  #pragma AP unroll
		  M[dep][i][col] = M[dep][i+1][col];
	  }

}

/* Line buffer shift down
 * Assumes new data pixel will be entered at the top of the line buffer
 * The bottom is row = LROW - 1
 */
template <typename T, int LDEP, int LROW, int LCOL>
  void t_linebuffer<T,LDEP,LROW,LCOL>::shift_down(int dep,int col)
{
#pragma AP inline
int i;

	  for(i = LROW-1; i > 0; i--){
	  #pragma AP unroll
		  M[dep][i][col] = M[dep][i+1][col];
	  }

}


/* Line buffer insert bottom
 * Inserts a new value in the bottom row of the line buffer at column = col
 * The bottom is row = 0
 */
template <typename T, int LDEP, int LROW, int LCOL>
  void t_linebuffer<T,LDEP,LROW,LCOL>::insert_bottom(T value,int dep, int col)
{
#pragma AP inline

  M[dep][LROW-1][col] = value;
}

/* Line buffer insert top
 * Inserts a new value in the top row of the line buffer at column = col
 * The bottom is row = LROW - 1
 */
template <typename T, int LDEP, int LROW, int LCOL>
  void t_linebuffer<T,LDEP,LROW,LCOL>::insert_top(T value, int dep ,int col)
{
#pragma AP inline

  M[dep][0][col] = value;
}

/* Line buffer getval
 * Returns the data value in the line buffer at position RowIndex, ColIndex
 */
template <typename T, int LDEP, int LROW, int LCOL>
  T t_linebuffer<T,LDEP,LROW,LCOL>::getval(int DepIdex,int RowIndex,int ColIndex)
{
#pragma AP inline

  T return_value;
  return_value = M[DepIdex][RowIndex][ColIndex];
  return return_value;
}

/* Memory window class definition */
template <typename T, int LDEP, int LROW, int LCOL>
  class t_window{
 public:
  T M[LDEP][LROW][LCOL];

  t_window(){
#pragma AP ARRAY_PARTITION variable=M dim=0 complete
  };
  ~t_window(){};
  void shift_right(int dep);
  void shift_left(int dep);
  void shift_up(int dep);
  void shift_down(int dep);
  void insert(T value,int dep,int row,int col);
  void print();
  T getval(int DepIndex,int RowIndex,int ColIndex);
};

/* Window print
 * Prints the entire contents of the memory window
 */
template <typename T, int LDEP, int LROW, int LCOL>
  void t_window<T,LDEP,LROW,LCOL>::print()
{
  int i, j;
  printf("Window Size = %d x %d\n",LROW,LCOL);
  printf("Col \t");
  for(j = 0; j < LCOL; j++){
    printf("%d \t",j);
  }
  printf("\n");
  for(i = 0; i < LROW; i++){
    printf("Row %d: \t",i);
    for(j=0; j < LCOL; j++){
      printf("%d\t",M[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}

/* Window shift right
 * Moves all the contents of the window horizontally
 * Assumes new values will be placed in column = LCOL-1
 */
template <typename T, int LDEP, int LROW, int LCOL>
  void t_window<T,LDEP,LROW,LCOL>::shift_left(int dep)
{
#pragma AP inline
  int i, j;
	  for(i = 0; i < LROW; i++){
#pragma AP unroll
		for(j=0; j < LCOL-1; j++){
#pragma AP unroll
		  M[dep][i][j] = M[dep][i][j+1];
		}
	  }

}

/* Window shift left
 * Moves all the contents of the window horizontally
 * Assumes new values will be placed in column = 0
 */
template <typename T, int LDEP, int LROW, int LCOL>
  void t_window<T,LDEP,LROW,LCOL>::shift_right(int dep)
{
#pragma AP inline
  int i, j;

 	#pragma AP unroll
	  for(i = 0; i < LROW; i++){
	#pragma AP unroll
		for(j=LCOL-1; j > 0; j--){
	#pragma AP unroll
		  M[dep][i][j] = M[dep][i][j-1];
		}
	  }

}

/* Window shift up
 * Moves all the contents of the window vertically
 * Assumes new values will be placed in row = 0
 */
template <typename T, int LDEP, int LROW, int LCOL>
  void t_window<T,LDEP,LROW,LCOL>::shift_down(int dep)
{
#pragma AP inline
  int i, j;

 #pragma AP unroll
	  for(i = LROW-1; i > 0; i--){
	#pragma AP unroll
		for(j=0; j < LCOL; j++){
	#pragma AP unroll
		  M[dep][i][j] = M[dep][i-1][j];
		}
	  }

}

/* Window shift down
 * Moves all the contents of the window vertically
 * Assumes new values will be placed in row = LROW - 1
 */
template <typename T, int LDEP, int LROW, int LCOL>
  void t_window<T,LDEP,LROW,LCOL>::shift_up(int dep)
{
#pragma AP inline
  int i, j;

	 #pragma AP unroll
	  for(i = 0; i < LROW-1; i++){
	#pragma AP unroll
		for(j=0; j < LCOL; j++){
	#pragma AP unroll
		  M[dep][i][j] = M[dep][i+1][j];
		}
	  }

}

/* Window insert
 * Inserts a new value at any location of the window
 */
template <typename T, int LDEP, int LROW, int LCOL>
  void t_window<T,LDEP,LROW,LCOL>::insert(T value, int dep, int row, int col)
{
#pragma AP inline
  M[dep][row][col] = value;
}

/* Window getval
 * Returns the value of any window location
 */
template <typename T, int LDEP, int LROW, int LCOL>
  T t_window<T,LDEP,LROW,LCOL>::getval(int DepIndex, int RowIndex, int ColIndex)
{
#pragma AP inline
  T return_value;
  return_value = M[DepIndex][RowIndex][ColIndex];
  return return_value;
}

#endif
