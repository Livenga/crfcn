#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/genetic.h"

void gcrossover(genotype *ch_gtype, genotype pr_gtype1, genotype pr_gtype2,
    const int population) {
  const int row = ((pr_gtype1[0] & 0x00FF) > (pr_gtype2[0] & 0x00FF)) ?
    (pr_gtype2[0] & 0x00FF) : (pr_gtype1[0] & 0x00FF);
  const int column = ((pr_gtype1[0] & 0x00FF00) > (pr_gtype2[0] & 0x00FF00)) ?
    ((pr_gtype2[0] & 0x00FF00) >> 8) : ((pr_gtype1[0] & 0x00FF00) >> 8);

  int i, j, pop, div_x, div_y;
  unsigned int gate;
  double m_rate, m_unit_rate;

  for(pop = 2; pop < population; pop++) {
    ch_gtype[pop][0] =
      (pr_gtype1[0] & 0x00FFFF) > (pr_gtype2[0] & 0x00FFFF) ?
      pr_gtype2[0] : pr_gtype1[0];

    div_x = (rand() % column);
    div_y = (rand() % row);

    gate  = get_gate(div_x);
    for(i = 1; i < div_y; i++)
      ch_gtype[pop][i] = (pr_gtype1[i] & gate) + (pr_gtype2[i] & ~gate);

    for(; i < row; i++) {
      ch_gtype[pop][i] = (pr_gtype1[i] & ~gate) + (pr_gtype2[i] & gate);
    }

    // 突然変異
    m_rate = (double)(rand() % 100) / 100.0;
    if(m_rate < MUTATION_RATE)
      gmutation(ch_gtype[pop]);
    
    // 突然変異(ユニット)
    m_unit_rate = (double)(rand() % 100) / 100.0;
    if(m_unit_rate < MUTATION_UNIT_RATE) {
      gunit_mutation(ch_gtype[pop]);
    }

    gate = get_gate(((ch_gtype[pop][0] >> 8) & 0xFF) + 2);
    for(j = 0; j < ch_gtype[pop][0] & 0xFF; j++)
      ch_gtype[pop][j] = ch_gtype[pop][j] & gate;
  }
}
