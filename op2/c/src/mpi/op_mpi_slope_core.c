
// mpi header
#include <mpi.h>

//#include <op_lib_core.h>
#include <op_lib_c.h>
#include <op_lib_mpi.h>
#include <op_util.h>

#include <op_mpi_core.h>

halo_list* OP_import_nonexec_ex_list;
halo_list* OP_export_nonexec_ex_list;

halo_list* OP_import_nonexec_merged_list;
halo_list* OP_export_nonexec_merged_list;

/*******************************************************************************
 * Routine to create an nonexec-import extended list (only a wrapper)
 *******************************************************************************/

static void create_nonexec_ex_import_list(op_set set, int *temp_list,
                                       halo_list h_list, int size,
                                       int comm_size, int my_rank) {
  create_export_list(set, temp_list, h_list, size, comm_size, my_rank);
}

/*******************************************************************************
 * Routine to create an nonexec-export extended list (only a wrapper)
 *******************************************************************************/

static void create_nonexec_ex_export_list(op_set set, int *temp_list,
                                       halo_list h_list, int total_size,
                                       int *ranks, int *sizes, int ranks_size,
                                       int comm_size, int my_rank) {
  create_import_list(set, temp_list, h_list, total_size, ranks, sizes,
                     ranks_size, comm_size, my_rank);
}

halo_list merge_halo_lists(halo_list h_list1, halo_list h_list2, int comm_size, int my_rank){
  if (compare_sets(h_list1->set, h_list2->set) != 1) {
    printf("ERROR: Invalid set merge");
    return NULL;
  }
  // printf("VALID set merge\n");

  int* h1_ranks = (int *)xmalloc(comm_size * sizeof(int));
  int* h2_ranks = (int *)xmalloc(comm_size * sizeof(int));

  int* h1_sizes = (int *)xmalloc(comm_size * sizeof(int));
  int* h2_sizes = (int *)xmalloc(comm_size * sizeof(int));

  int* h1_disps = (int *)xmalloc(comm_size * sizeof(int));
  int* h2_disps = (int *)xmalloc(comm_size * sizeof(int));

  for(int i = 0; i < comm_size; i ++){
    h1_ranks[i] = -1;
    h2_ranks[i] = -1;
    h1_sizes[i] = -1;
    h2_sizes[i] = -1;
    h1_disps[i] = -1;
    h2_disps[i] = -1;
  }

  for(int i = 0; i < h_list1->ranks_size; i++){
    h1_ranks[h_list1->ranks[i]] = 1;
    h1_sizes[h_list1->ranks[i]] = h_list1->sizes[i];
    h1_disps[h_list1->ranks[i]] = h_list1->disps[i];
  }

  for(int i = 0; i < h_list2->ranks_size; i++){
    h2_ranks[h_list2->ranks[i]] = 1;
    h2_sizes[h_list2->ranks[i]] = h_list2->sizes[i];
    h2_disps[h_list2->ranks[i]] = h_list2->disps[i];
  }

  //count cumulative size
  int merged_size = 0;
  for(int i = 0; i < comm_size; i++){
    if(h1_ranks[i] > 0){
      merged_size++;
    }
    else if(h2_ranks[i] > 0){
      merged_size++;
    }
  }

  // if(merged_size == 0){
  //   printf("EMPTY merged_size=%d %d %d\n", merged_size, h_list1->ranks_size, h_list2->ranks_size);
  // }

  int *ranks = (int *)xmalloc(comm_size * sizeof(int));
  int *disps = (int *)xmalloc(comm_size * sizeof(int));
  int *sizes = (int *)xmalloc(comm_size * sizeof(int));

  int index = 0;
  int total_size = 0;
  for(int i = 0; i < comm_size; i++){
    int rank = -1;
    int size = 0;
    if(h1_ranks[i] > 0){
      rank = i;
      size += h1_sizes[i];
      total_size += h1_sizes[i];
    }
    if(h2_ranks[i] > 0){
      rank = i;
      size += h2_sizes[i];
      total_size += h2_sizes[i];
    }

    if(rank >= 0){
      ranks[index] = rank;
      sizes[index] = size;
      index++;
    }
  }

  disps[0] = 0;
  for (int i = 0; i < comm_size; i++) {
    if (i > 0)
      disps[i] = disps[i - 1] + sizes[i - 1];
  }

  int* list = (int *)xmalloc(total_size * sizeof(int));

  int disp = 0;
  int start = 0;
  index = 0;

  for(int i = 0; i < comm_size; i++){
    int rank = -1;
    int size = 0;
    start = disp;
    if(h1_ranks[i] > 0){
      for(int j = 0; j < h1_sizes[i]; j++){
        list[disp++] = h_list1->list[h1_disps[i] + j];
      }
    }
    if(h2_ranks[i] > 0){
      for(int j = 0; j < h2_sizes[i]; j++){
        list[disp++] = h_list2->list[h2_disps[i] + j];
      }
    }

    if((disp - 1) > 0)
      quickSort(list, start, (disp - 1) < 0 ? 0 : (disp - 1));
  }

  halo_list h_list = (halo_list)xmalloc(sizeof(halo_list_core));

  h_list->set = h_list1->set;
  h_list->size = total_size;
  h_list->ranks = ranks;
  h_list->ranks_size = merged_size;
  h_list->disps = disps;
  h_list->sizes = sizes;
  h_list->list = list;

  return h_list;
}

void step5_1(int **part_range, int my_rank, int comm_size){

  for (int m = 0; m < OP_map_index; m++) { // for each maping table
    
    op_map map = OP_map_list[m];
    halo_list i_list = OP_import_nonexec_list[map->from->index];
    halo_list e_list = OP_export_nonexec_list[map->from->index];

    MPI_Request request_send[e_list->ranks_size];

    // prepare bits of the mapping tables to be exported
    int **sbuf = (int **)xmalloc(e_list->ranks_size * sizeof(int *));
    for (int i = 0; i < e_list->ranks_size; i++) {
      sbuf[i] = (int *)xmalloc(e_list->sizes[i] * map->dim * sizeof(int));
      for (int j = 0; j < e_list->sizes[i]; j++) {
        for (int p = 0; p < map->dim; p++) {
          sbuf[i][j * map->dim + p] =
              map->map[map->dim * (e_list->list[e_list->disps[i] + j]) + p];
        }
      }
      // printf("\n export from %d to %d map %10s, number of elements of size %d
      // | sending:\n ",
      //    my_rank,e_list.ranks[i],map.name,e_list.sizes[i]);
      MPI_Isend(sbuf[i], map->dim * e_list->sizes[i], MPI_INT, e_list->ranks[i],
                m, OP_MPI_WORLD, &request_send[i]);
    }

    // prepare space for the incomming mapping tables - realloc each
    // mapping tables in each mpi process
    int* temp = (int *)xrealloc(
        OP_map_list[map->index]->map,
        (map->dim * (map->from->size +  OP_import_exec_list[map->from->index]->size + i_list->size)) * sizeof(int));
    OP_map_list[map->index]->map = temp;

    int init = map->dim * (map->from->size + OP_import_exec_list[map->from->index]->size);

    for (int i = 0; i < i_list->ranks_size; i++) {
      // printf("\n imported on to %d map %10s, number of elements of size %d |
      // recieving: ",
      // my_rank, map->name, i_list->size);
      MPI_Recv(
          &(OP_map_list[map->index]->map[init + i_list->disps[i] * map->dim]),
          map->dim * i_list->sizes[i], MPI_INT, i_list->ranks[i], m,
          OP_MPI_WORLD, MPI_STATUS_IGNORE);
    }
    MPI_Waitall(e_list->ranks_size, request_send, MPI_STATUSES_IGNORE);
    for (int i = 0; i < e_list->ranks_size; i++)
      op_free(sbuf[i]);
    op_free(sbuf);
  }
}

void step7_1(int **part_range, int my_rank, int comm_size){

  OP_import_nonexec_ex_list =
      (halo_list *)xmalloc(OP_set_index * sizeof(halo_list));
  OP_export_nonexec_ex_list =
      (halo_list *)xmalloc(OP_set_index * sizeof(halo_list));

  OP_import_nonexec_merged_list =
      (halo_list *)xmalloc(OP_set_index * sizeof(halo_list));
  OP_export_nonexec_merged_list =
      (halo_list *)xmalloc(OP_set_index * sizeof(halo_list));
  
  // declare temporaty scratch variables to hold non-exec set export lists
  int s_i = 0;
  int *set_list = NULL;

  int cap_s = 1000; // keep track of the temp array capacities

  for (int s = 0; s < OP_set_index; s++) { // for each set
    op_set set = OP_set_list[s];
    halo_list exec_set_list = OP_import_nonexec_list[set->index];

    // create a temporaty scratch space to hold nonexec export list for this set
    s_i = 0;
    set_list = (int *)xmalloc(cap_s * sizeof(int));

    for (int m = 0; m < OP_map_index; m++) { // for each maping table
      op_map map = OP_map_list[m];
      halo_list exec_map_list = OP_import_exec_list[map->from->index];
      halo_list nonexec_map_list = OP_import_nonexec_list[map->from->index];

      if (compare_sets(map->to, set) == 1) { // need to select
                                             // mappings TO this set

        // for each entry in this mapping table: original+execlist
        int len1 = map->from->size + exec_map_list->size;
        int len = map->from->size + exec_map_list->size + nonexec_map_list->size;
        for (int e = len1; e < len; e++) {
          int part;
          int local_index;
          for (int j = 0; j < map->dim; j++) { // for each element pointed
                                               // at by this entry
            part = get_partition(map->map[e * map->dim + j],
                                 part_range[map->to->index], &local_index,
                                 comm_size);

            if (s_i >= cap_s) {
              cap_s = cap_s * 2;
              set_list = (int *)xrealloc(set_list, cap_s * sizeof(int));
            }

            if (part != my_rank) {
              int found = -1;
              // check in exec list
              int rank = binary_search(exec_set_list->ranks, part, 0,
                                       exec_set_list->ranks_size - 1);

              if (rank >= 0) {
                found = binary_search(exec_set_list->list, local_index,
                                      exec_set_list->disps[rank],
                                      exec_set_list->disps[rank] +
                                          exec_set_list->sizes[rank] - 1);
              }

              if (found < 0) {
                // not in this partition and not found in
                // exec list
                // add to non-execute set_list
                set_list[s_i++] = part;
                set_list[s_i++] = local_index;
              }
            }
          }
        }
      }
    }

    // create non-exec set import list
    halo_list h_list = (halo_list)xmalloc(sizeof(halo_list_core));
    create_nonexec_ex_import_list(set, set_list, h_list, s_i, comm_size, my_rank);
    op_free(set_list); // free temp list
    OP_import_nonexec_ex_list[set->index] = h_list;
  }

  for (int s = 0; s < OP_set_index; s++) { // for each set
    op_set set = OP_set_list[s];

    halo_list h_list = OP_import_nonexec_list[set->index];
    halo_list h_list_ex = OP_import_nonexec_ex_list[set->index];
    halo_list h_list_merged = merge_halo_lists(h_list, h_list_ex, comm_size, my_rank);
    OP_import_nonexec_merged_list[set->index] = h_list_merged;
    OP_import_nonexec_list[set->index] = h_list_merged;  //suneth
  }

}

void step7_2(int **part_range, int my_rank, int comm_size){

  int *neighbors, *sizes;
  int ranks_size;

  for (int s = 0; s < OP_set_index; s++) { // for each set
    op_set set = OP_set_list[s];

    //-----Discover neighbors-----
    ranks_size = 0;
    neighbors = (int *)xmalloc(comm_size * sizeof(int));
    sizes = (int *)xmalloc(comm_size * sizeof(int));

    halo_list list = OP_import_nonexec_ex_list[set->index];
    find_neighbors_set(list, neighbors, sizes, &ranks_size, my_rank, comm_size,
                       OP_MPI_WORLD);

    MPI_Request request_send[list->ranks_size];
    int *rbuf, cap = 0, index = 0;

    for (int i = 0; i < list->ranks_size; i++) {
      int *sbuf = &list->list[list->disps[i]];
      MPI_Isend(sbuf, list->sizes[i], MPI_INT, list->ranks[i], s, OP_MPI_WORLD,
                &request_send[i]);
    }

    for (int i = 0; i < ranks_size; i++)
      cap = cap + sizes[i];
    int *temp = (int *)xmalloc(cap * sizeof(int));

    // export this list to those neighbors
    for (int i = 0; i < ranks_size; i++) {
      // printf("export to %d from %d set %10s, list of size %d | recieving:\n",
        //  neighbors[i], my_rank, set->name, sizes[i]);
      rbuf = (int *)xmalloc(sizes[i] * sizeof(int));
      MPI_Recv(rbuf, sizes[i], MPI_INT, neighbors[i], s, OP_MPI_WORLD,
               MPI_STATUS_IGNORE);
      memcpy(&temp[index], (void *)&rbuf[0], sizes[i] * sizeof(int));
      index = index + sizes[i];
      op_free(rbuf);
    }

    MPI_Waitall(list->ranks_size, request_send, MPI_STATUSES_IGNORE);

    // create import lists
    halo_list h_list = (halo_list)xmalloc(sizeof(halo_list_core));
    create_nonexec_ex_export_list(set, temp, h_list, index, neighbors, sizes,
                               ranks_size, comm_size, my_rank);
    
    OP_export_nonexec_ex_list[set->index] = h_list;
  }

   for (int s = 0; s < OP_set_index; s++) { // for each set
    op_set set = OP_set_list[s];

    halo_list h_list = OP_export_nonexec_list[set->index];
    halo_list h_list_ex = OP_export_nonexec_ex_list[set->index];
    halo_list h_list_merged = merge_halo_lists(h_list, h_list_ex, comm_size, my_rank);
    OP_export_nonexec_merged_list[set->index] = h_list_merged;
    OP_export_nonexec_list[set->index] = h_list_merged;  //suneth
  }
}

void step7_3(int **part_range, int my_rank, int comm_size){
    for (int s = 0; s < OP_set_index; s++) { // for each set
    op_set set = OP_set_list[s];

    halo_list i_list = OP_import_nonexec_list[set->index];
    halo_list e_list = OP_export_nonexec_list[set->index];

    // for each data array
    op_dat_entry *item;
    int d = -1; // d is just simply the tag for mpi comms
    TAILQ_FOREACH(item, &OP_dat_list, entries) {
      d++; // increase tag to do mpi comm for the next op_dat
      op_dat dat = item->dat;

      if (compare_sets(set, dat->set) == 1) { // if this data array is
                                              // defined on this set

        MPI_Request request_send[e_list->ranks_size];

        // prepare non-execute set element data to be exported
        char **sbuf = (char **)xmalloc(e_list->ranks_size * sizeof(char *));

        for (int i = 0; i < e_list->ranks_size; i++) {
          sbuf[i] = (char *)xmalloc(e_list->sizes[i] * dat->size);
          for (int j = 0; j < e_list->sizes[i]; j++) {
            int set_elem_index = e_list->list[e_list->disps[i] + j];
            memcpy(&sbuf[i][j * dat->size],
                   (void *)&dat->data[dat->size * (set_elem_index)], dat->size);
          }
          //  printf("nonexec export from %d to %d data %10s, number of elements of size %d | sending:\n ",
          //    my_rank,e_list->ranks[i],dat->name,e_list->sizes[i]);
          MPI_Isend(sbuf[i], dat->size * e_list->sizes[i], MPI_CHAR,
                    e_list->ranks[i], d, OP_MPI_WORLD, &request_send[i]);
        }


        // prepare space for the incomming nonexec-data - realloc each
        // data array in each mpi process
        halo_list exec_i_list = OP_import_exec_list[set->index];

        dat->data = (char *)xrealloc(
            dat->data,
            (set->size + exec_i_list->size + i_list->size) * dat->size);

        int init = (set->size + exec_i_list->size) * dat->size;
        for (int i = 0; i < i_list->ranks_size; i++) {
          MPI_Recv(&(dat->data[init + i_list->disps[i] * dat->size]),
                   dat->size * i_list->sizes[i], MPI_CHAR, i_list->ranks[i], d,
                   OP_MPI_WORLD, MPI_STATUS_IGNORE);
        }

        MPI_Waitall(e_list->ranks_size, request_send, MPI_STATUSES_IGNORE);
        for (int i = 0; i < e_list->ranks_size; i++)
          op_free(sbuf[i]);
        op_free(sbuf);
        // printf("nonexec imported on to %d data %10s, number of elements of size %d start=%d | recieving:\n ",
        //    my_rank, dat->name, i_list->size, init/dat->size);
      }
    }
  }
}

void step8_1(int **part_range, int my_rank, int comm_size){
  for (int s = 0; s < OP_set_index; s++) { // for each set
    op_set set = OP_set_list[s];

    for (int m = 0; m < OP_map_index; m++) { // for each maping table
      op_map map = OP_map_list[m];

      if (compare_sets(map->to, set) == 1) { // need to select
                                             // mappings TO this set

        halo_list exec_set_list = OP_import_exec_list[set->index];
        halo_list nonexec_set_list = OP_import_nonexec_list[set->index];

        halo_list exec_map_list = OP_import_exec_list[map->from->index];
        halo_list nonexec_map_list = OP_import_nonexec_list[map->from->index];

        // for each entry in this mapping table: original+execlist
        int len = map->from->size + exec_map_list->size + nonexec_map_list->size;
        for (int e = 0; e < len; e++) {
          for (int j = 0; j < map->dim; j++) { // for each element
                                               // pointed at by this entry
            int part;
            int local_index = 0;
            part = get_partition(map->map[e * map->dim + j],
                                 part_range[map->to->index], &local_index,
                                 comm_size);

            if (part == my_rank) {
              OP_map_list[map->index]->map[e * map->dim + j] = local_index;
            } else {
              int found = -1;
              // check in exec list
              int rank1 = binary_search(exec_set_list->ranks, part, 0,
                                        exec_set_list->ranks_size - 1);
              // check in nonexec list
              int rank2 = binary_search(nonexec_set_list->ranks, part, 0,
                                        nonexec_set_list->ranks_size - 1);

              if (rank1 >= 0) {
                found = binary_search(exec_set_list->list, local_index,
                                      exec_set_list->disps[rank1],
                                      exec_set_list->disps[rank1] +
                                          exec_set_list->sizes[rank1] - 1);
                if (found >= 0) {
                  OP_map_list[map->index]->map[e * map->dim + j] =
                      found + map->to->size;
                }
              }

              if (rank2 >= 0 && found < 0) {
                found = binary_search(nonexec_set_list->list, local_index,
                                      nonexec_set_list->disps[rank2],
                                      nonexec_set_list->disps[rank2] +
                                          nonexec_set_list->sizes[rank2] - 1);
                if (found >= 0) {
                  OP_map_list[map->index]->map[e * map->dim + j] =
                      found + set->size + exec_set_list->size;
                }
              }

              if (found < 0)
                printf("ERROR: Set %10s Element %d needed on rank %d \
                    from partition %d\n",
                       set->name, local_index, my_rank, part);
            }
          }
        }
      }
    }
  }
}
/*******************************************************************************
 * Main MPI halo creation routine
 *******************************************************************************/

void op_halo_create_slope() {

  // declare timers
  double cpu_t1, cpu_t2, wall_t1, wall_t2;
  double time;
  double max_time;
  op_timers(&cpu_t1, &wall_t1); // timer start for list create

  // create new communicator for OP mpi operation
  int my_rank, comm_size;
  // MPI_Comm_dup(OP_MPI_WORLD, &OP_MPI_WORLD);
  MPI_Comm_rank(OP_MPI_WORLD, &my_rank);
  MPI_Comm_size(OP_MPI_WORLD, &comm_size);

  /* Compute global partition range information for each set*/
  int **part_range = (int **)xmalloc(OP_set_index * sizeof(int *));
  get_part_range(part_range, my_rank, comm_size, OP_MPI_WORLD);

  // save this partition range information if it is not already saved during
  // a call to some partitioning routine
  if (orig_part_range == NULL) {
    orig_part_range = (int **)xmalloc(OP_set_index * sizeof(int *));
    for (int s = 0; s < OP_set_index; s++) {
      op_set set = OP_set_list[s];
      orig_part_range[set->index] = (int *)xmalloc(2 * comm_size * sizeof(int));
      for (int j = 0; j < comm_size; j++) {
        orig_part_range[set->index][2 * j] = part_range[set->index][2 * j];
        orig_part_range[set->index][2 * j + 1] =
            part_range[set->index][2 * j + 1];
      }
    }
  }

  /*----- STEP 1 - Construct export lists for execute set elements and related
    mapping table entries -----*/
  step1(part_range, my_rank, comm_size);
  
  /*---- STEP 2 - construct import lists for mappings and execute sets------*/
  step2(part_range, my_rank, comm_size);
  
  /*--STEP 3 -Exchange mapping table entries using the import/export lists--*/
  step3(part_range, my_rank, comm_size);

  /*-- STEP 4 - Create import lists for non-execute set elements using mapping
    table entries including the additional mapping table entries --*/
  step4(part_range, my_rank, comm_size);

  /*----------- STEP 5 - construct non-execute set export lists -------------*/
  step5(part_range, my_rank, comm_size);
  step5_1(part_range, my_rank, comm_size);

  /*-STEP 6 - Exchange execute set elements/data using the import/export
   * lists--*/
  step6(part_range, my_rank, comm_size);
  
  /*-STEP 7 - Exchange non-execute set elements/data using the import/export
   * lists--*/
  step7_1(part_range, my_rank, comm_size);
  step7_2(part_range, my_rank, comm_size);
  step7_3(part_range, my_rank, comm_size);

  /*-STEP 8 ----------------- Renumber Mapping tables-----------------------*/
  step8_1(part_range, my_rank, comm_size);

  /*-STEP 9 ---------------- Create MPI send Buffers-----------------------*/
  step9(part_range, my_rank, comm_size);

  /*-STEP 10 -------------------- Separate core
   * elements------------------------*/
  int **core_elems = (int **)xmalloc(OP_set_index * sizeof(int *));
  int **exp_elems = (int **)xmalloc(OP_set_index * sizeof(int *));
  step10(part_range, core_elems, exp_elems, my_rank, comm_size);

  /*-STEP 11 ----------- Save the original set element
   * indexes------------------*/
  step11(part_range, core_elems, exp_elems, my_rank, comm_size);

  /*-STEP 12 ---------- Clean up and Compute rough halo size
   * numbers------------*/
  for (int i = 0; i < OP_set_index; i++) {
    op_free(part_range[i]);
    op_free(core_elems[i]);
    op_free(exp_elems[i]);
  }
  op_free(part_range);
  op_free(exp_elems);
  op_free(core_elems);

  op_timers(&cpu_t2, &wall_t2); // timer stop for list create
  // compute import/export lists creation time
  time = wall_t2 - wall_t1;
  MPI_Reduce(&time, &max_time, 1, MPI_DOUBLE, MPI_MAX, MPI_ROOT, OP_MPI_WORLD);

  step12(part_range, max_time, my_rank, comm_size);

}

void op_single_halo_destroy(halo_list* h_list){

   for (int s = 0; s < OP_set_index; s++) {
    op_set set = OP_set_list[s];

    op_free(h_list[set->index]->ranks);
    op_free(h_list[set->index]->disps);
    op_free(h_list[set->index]->sizes);
    op_free(h_list[set->index]->list);
    op_free(h_list[set->index]);
  }
  op_free(h_list);
}

void op_halo_destroy_slope() {

  op_halo_destroy();

  op_single_halo_destroy(OP_import_nonexec_ex_list);
  op_single_halo_destroy(OP_export_nonexec_ex_list);
  op_single_halo_destroy(OP_import_nonexec_merged_list);
  op_single_halo_destroy(OP_export_nonexec_merged_list);
}