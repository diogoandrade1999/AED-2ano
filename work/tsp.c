//
// Student name
// Student name
// ...
//
// AED, 2018/2019
//
// solution of the traveling salesman problem
//

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "cities.h"
#include "elapsed_time.h"

#define SAVE_DATA 1

//
// record best solutions
//

static int min_length, max_length;
static int min_tour[max_n_cities + 1], max_tour[max_n_cities + 1];
static long n_tours;
#if SAVE_DATA
  static FILE *fp;
  static int size_data = 1;
  static int *data;
#endif

//
// first solution (brute force, distance computed at the end, compute best and worst tours)
//

void tsp_v1(int n, int m, int *a)
{
  int i, t;

  if (m < n - 1)
    for (i = m; i < n; i++)
    {
      t = a[m];
      a[m] = a[i];
      a[i] = t;
      tsp_v1(n, m + 1, a);
      t = a[m];
      a[m] = a[i];
      a[i] = t;
    }
  else
  { // visit permutation
    int actual_sum = 0;
    for (int num = 0; num < n - 1; num++)
      actual_sum += cities[a[num]].distance[a[num + 1]];
    actual_sum += cities[a[n - 1]].distance[a[0]];

#if SAVE_DATA
    if (n == 12 || n == 15){
      if(size_data<=actual_sum){
        int *new_data = data;
        data = malloc(actual_sum*sizeof(int));
        memcpy(data, new_data, actual_sum*sizeof(int));
        free(new_data);
        for(int num = size_data; num<actual_sum; num++)
          data[num] = 0;
        size_data = actual_sum;
      }
      data[actual_sum-1]++;   
    }
    //  fprintf(fp, "%d ", actual_sum);
#endif

    if (actual_sum < min_length)
    {
      min_length = actual_sum;
      memcpy(min_tour, a, n * sizeof(int));
    }
    if (actual_sum > max_length)
    {
      max_length = actual_sum;
      memcpy(max_tour, a, n * sizeof(int));
    }
    n_tours++;
  }
}

//
// main program
//

int main(int argc, char **argv)
{

  int n_mec, special, n, i, a[max_n_cities];
  char file_name[32];
  double dt1;

  n_mec = 89265;
  special = 0;
  init_cities_data(n_mec, special);

  FILE *resultado = fopen("resultado.txt", "w");
  fprintf(resultado, "data for init_cities_data(%d,%d)\n", n_mec, special);
  fflush(stdout);
#if SAVE_DATA
  fp = fopen("data.txt", "w");
  data = malloc(1*sizeof(int));
  data[0] = 0;
#endif
#if 1
  print_distances();
#endif
  //inicio do print da tabela
  fprintf(resultado, "%3s | %10s %*s %*s %10s %*s\n", "n", "minLength", 54 / 2 + (int)strlen("minPath") / 2, "minPath", 54 / 2 - (int)strlen("minPath") / 2 + 1, "|", "maxLength", 54 / 2 + (int)strlen("maxPath") / 2, "maxPath");
  fclose(resultado);
  for (n = 3; n <= n_cities; n++)
  {
    FILE *resultado = fopen("resultado.txt", "a");
    //
    // try tsp_v1
    //
    dt1 = -1.0;
    if (n <= 15)
    {
      (void)elapsed_time();
      for (i = 0; i < n; i++)
        a[i] = i;
      min_length = 1000000000;
      max_length = 0;
      n_tours = 0l;

      tsp_v1(n, 1, a); // no need to change the starting city, as we are making a tour

      dt1 = elapsed_time();
#if SAVE_DATA
      if (n == 12 || n == 15){
        for(int num=0; num<size_data; num++)
          fprintf(fp, "%d %d\n", num+1, data[num]);
        fprintf(fp, "\n");
        //inicializar para a nova contagem
        data = malloc(1*sizeof(int));
        data[0] = 0;
        size_data = 0;
      }
#endif
      //print das tabelas como o exemplo dos slides

      fprintf(resultado, "%-3d | %-10d", n, min_length);
      //printf("tsp_v1() finished in %8.3fs (%ld tours generated)\n",dt1,n_tours);
      //printf("  min %5d [",min_length);
      fprintf(resultado, "%*s", 54 / 2 - 3 * n / 2, " ");
      for (i = 0; i < n; i++)
        fprintf(resultado, "%3d", min_tour[i]);
      fprintf(resultado, "%*s", 54 / 2 - 3 * n / 2 + 3, "|");
      fprintf(resultado, "%-10d", max_length);
      //printf("  max %5d [",max_length);
      fprintf(resultado, "%*s", 54 / 2 - 3 * n / 2, " ");
      for (i = 0; i < n; i++)
        fprintf(resultado, "%3d", max_tour[i]);
      fprintf(resultado, "%*s", 54 / 2 - 3 * n / 2 + 3, "|");
      fflush(stdout);
      if (argc == 2 && strcmp(argv[1], "-f") == 0)
      {
        min_tour[n] = -1;
        sprintf(file_name, "min_%02d.svg", n);
        make_map(file_name, min_tour);
        max_tour[n] = -1;
        sprintf(file_name, "max_%02d.svg", n);
        make_map(file_name, max_tour);
      }
    }
    fprintf(resultado, "tsp_v1() finished in %8.7fs (%ld tours generated)", dt1, n_tours);
    fprintf(resultado, "\n");
    fclose(resultado);
  }
#if SAVE_DATA
  fclose(fp);
#endif
  //system("poweroff");
  return 0;
}
