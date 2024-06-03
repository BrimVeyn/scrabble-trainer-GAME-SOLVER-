/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 13:23:41 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/05/28 10:25:22 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/struct.h"
#include "include/Vector/vector_define.h"
#include "include/Iterator/iterator_define.h"

// int main(void) {
// 	Vector vect = vector_construct(INT_TYPE);
//
// 	vector_push_back(&vect, INT_L(12));
// 	int NB_OF_EL = 500;
//
// 	for (int i = 0; i < NB_OF_EL; i++) {
// 		int random_number = rand() % 2000;
// 		vector_push_back(&vect, &random_number); 
// 	}
// 	
// 	vector_quick_sort(&vect);
//
// 	for (Iterator it = it_begin(&vect); IT_NEQ(it, it_end(&vect)); it_pp(&it)) {
// 		it_print(&it);
// 		it_add(&it, INT_L(100));
// 		it_print(&it);
// 	}
//
// 	vector_destruct(&vect);
//
// 	// vector_destruct(&vect2);
//
// 	// Vector vect2 = vector_construct(INT_TYPE);
// 	//
// 	// vector_push_back(&vect2, INT_L(12));
// 	// vector_push_back(&vect2, INT_L(250));
// 	// vector_push_back(&vect2, INT_L(1289));
// 	// vector_push_back(&vect2, INT_L(12910));
// 	// vector_push_back(&vect2, INT_L(322));
// 	// vector_push_back(&vect2, INT_L(1212));
// 	// vector_push_back(&vect2, INT_L(3039));
// 	// vector_push_back(&vect2, INT_L(1020));
// 	//
// 	// vector_quick_sort(&vect2);
// 	//
// 	// // vector_print(&vect2);
// 	// Iterator it_p = vector_ternary_search(&vect2, INT_L(250));
// 	// Iterator it_p2 = vector_binary_search(&vect2, INT_L(250));
// 	// Iterator it_p3 = vector_binary_search(&vect2, INT_L(12911));
// 	//
// 	//
// 	// it_print(&it_p2);
// 	// it_print(&it_p3);
// 	// it_print(&it_p);
// 	//
// 	// 
// 	// vector_destruct(&vect2);
//
// }




int main(void) {


	int NB_OF_EL = 40000;
	Vector vect = vector_construct(STR_TYPE);
	Vector vect_2 = vector_construct(STR_TYPE);
	srand(time(0));


	long start = clock();
	char a[8];
	a[7] = 0;

	for (int i = 0; i < NB_OF_EL; i++) {
		for (int j = 0; j < 8; j++) {
			a[j] = 'a' + (rand() % 25);
		}
		vector_push_back(&vect, &a); 
	}

	vector_quick_sort(&vect);
	// vector_print(&vect);
	printf("CTime = {%zu} %d str el quick_sort\n", (clock() - start), NB_OF_EL);

	start = clock();
	char b[8];
	b[7] = 0;

	for (int i = 0; i < NB_OF_EL; i++) {
		for (int j = 0; j < 8; j++) {
			b[j] = 'a' + (rand() % 25);
		}
		vector_push_sorted(&vect_2, &b); 
	}

	// vector_quick_sort(&vect);
	// vector_print(&vect);
	printf("CTime = {%zu} %d str el insert_sort\n", (clock() - start), NB_OF_EL);

	Vector vect_3 = vector_construct(INT_TYPE);
	Vector vect_4 = vector_construct(INT_TYPE);
	start = clock();

	for (int i = 0; i < NB_OF_EL; i++) {
		int random_number = rand() % 2000;
		vector_push_back(&vect_4, &random_number); 
	}

	vector_quick_sort(&vect_4);
	printf("CTime = {%zu} %d int el quick_sort\n", (clock() - start), NB_OF_EL);
	// vector_print(&vect_4);

	start = clock();

	for (int i = 0; i < NB_OF_EL; i++) {
		int random_number = rand() % 2000;
		vector_push_sorted(&vect_3, &random_number); 
	}

	printf("CTime = {%zu} %d int el insert_sort\n", (clock() - start), NB_OF_EL);
	vector_destruct(&vect);
}

// int main(void) {
//
// 	Vector int_vect = vector_construct_size(40, INT_TYPE);
//
// 	vector_push_back(&int_vect, INT_L(12));
// 	vector_push_back(&int_vect, INT_L(12));
// 	vector_push_back(&int_vect, INT_L(12));
// 	vector_push_back(&int_vect, INT_L(15));
// 	vector_push_back(&int_vect, INT_L(12));
// 	vector_push_back(&int_vect, INT_L(12));
//
// 	vector_print(&int_vect);
//
// 	printf("size = %zu\n", vector_get_size(&int_vect));
// 	printf("capacity = %zu\n", vector_get_capacity(&int_vect));
//
// 	Iterator it = vector_find(&int_vect, INT_L(15));
//
// 	it_add(&it, INT_L(100));
// 	// vector_clear(&int_vect);
// 	// vector_print(&int_vect);
//
// 	printf("begin = %zu, end = %zu\n", it_begin(&int_vect).index, it_end(&int_vect).index);
//
// 	printf("###########################\n");
// 	for (; IT_NEQ(it, it_end(&int_vect));) {
// 		it = vector_find(&int_vect, INT_L(12));
// 		it_erase(&it);
// 	}
// 	printf("###########################\n");
//
// 	vector_print(&int_vect);
// 	vector_destruct(&int_vect);
// }

// int main(void) {
//
// 	Vector int_vect = vector_construct();
// 	int tab[4] = { 1, 2, 3, 4 };
//
// 	vector_push_back(&int_vect, &tab[0], int_dup);
// 	vector_push_back(&int_vect, &tab[1], int_dup);
//
// 	vector_print(&int_vect, int_print);
//
// 	Vector str_vect = vector_construct();
// 	char *str_tab[5] = { "wonder", "happens", "sometimes", "inthisworld" };
//
// 	vector_push_back(&str_vect, str_tab[0], str_dup);
// 	vector_push_back(&str_vect, str_tab[1], str_dup);
// 	vector_push_back(&str_vect, str_tab[2], str_dup);
// 	vector_push_back(&str_vect, str_tab[3], str_dup);
//
// 	vector_print(&str_vect, str_print);
//
// 	for (size_t i = 0; i < vector_get_size(&str_vect); i++) {
// 		char *value = vector_at(&str_vect, i);
// 		printf("test_at[%zu] = %s\n", i, value);
// 	}
//
// 	vector_pop_back(&str_vect);
//
// 	char *test __attribute__((unused)) = vector_at(&str_vect, 12);
//
// 	char *test_front = vector_front(&str_vect);
// 	char *test_back = vector_back(&str_vect);
//
// 	printf("Vector_front = %s\n", test_front);
// 	printf("Vector_back = %s\n", test_back);
//
// 	Vector empty_vect = vector_construct();
//
// 	char *test_empty __attribute__((unused)) = vector_front(&empty_vect);
// 	char *test2_empty __attribute__((unused)) = vector_back(&empty_vect);
//
//
// 	printf("-----------------\n");
// 	vector_print(&str_vect, str_print);
// 	printf("-----------------\n");
// 	vector_erase_index(&str_vect, 0);
// 	vector_erase_index(&str_vect, 0);
// 	vector_erase_index(&str_vect, 0);
// 	vector_erase_index(&str_vect, 0);
// 	vector_erase_index(&str_vect, 0);
// 	printf("-----------------\n");
// 	vector_print(&str_vect, str_print);
// 	printf("-----------------\n");
//
// 	vector_destruct(&int_vect);
// 	vector_destruct(&str_vect);
// 	vector_destruct(&empty_vect);
// }
