/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 13:33:12 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/05/27 14:33:44 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file vector.h
 * @brief A simple vector library.
 */

#ifndef VECT_STRUCT_H
# define VECT_STRUCT_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

typedef void *(*dup_func)(const void *);
typedef void (*print_func)(size_t, const void *);
typedef void (*add_func)(void **, void *);
typedef int (*cmp_func)(void *, void *);

#define INT_L(value)  (&(int){value})
#define STR_L(str)  (&(char[]){str})

#define IT_NEQ(it1, it2) ((it1).vector == (it2).vector && (it1).index != (it2).index)
#define IT_EQ(it1, it2) ((it1).vector == (it2).vector && (it1).index == (it2).index)

#define Vector struct s_vector
#define Iterator struct s_vector_iterator

#define VECTOR_DEFAULT_SIZE 0
#define VECTOR_DEFAULT_CAPACITY 10
#define VECTOR_NULL 1

#ifndef SILENT
# define SILENT 0
#endif

/**
 * @struct t_vector
 * @brief A dynamic array (vector) data structure.
 *
 * The t_vector struct represents a dynamic array (vector) data structure
 * that can hold elements of any data type. It dynamically grows in size as
 * elements are added to it.
 */

typedef struct s_util_fun {

    dup_func    duplicate;
    print_func  print;
    add_func	add;
	cmp_func	cmp;
	char		type;

} t_util_fun;

#define INT_TYPE (struct s_util_fun){ int_dup, int_print, int_plus, int_cmp, 1 }
#define STR_TYPE (struct s_util_fun){ str_dup, str_print, str_plus, str_cmp, 2 }

typedef struct s_vector {
	
    void		**data;     /**< Pointer to the array storing the elements. */
    size_t		size;     /**< Number of elements currently stored in the vector. */
    size_t		capacity; /**< Current capacity of the vector (number of allocated slots). */
	struct s_util_fun	funcs;
	
} t_vector;

typedef struct s_vector_iterator {

    struct s_vector *vector;
    size_t index;

} t_vector_iterator;

#endif
