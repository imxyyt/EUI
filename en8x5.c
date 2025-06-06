#include "en8x5.h"

const u8 en_char[96][EN_CHAR_W]={//0x20+
  {0x00,0x00,0x00,0x00,0x00},//[ ]
  {0x00,0x00,0xbe,0x00,0x00},//[!]
  {0x06,0x00,0x00,0x06,0x00},//["]
  {0x28,0xfe,0x28,0xfe,0x28},//[#]
  {0x4c,0x92,0xfe,0x92,0x64},//[$]
  {0x84,0x60,0x10,0x0c,0x42},//[%]
  {0x74,0x8a,0x92,0xa8,0x40},//[&]
  {0x00,0x00,0x06,0x00,0x00},//[']
  {0x00,0x00,0x7c,0x82,0x00},//[(]
  {0x00,0x00,0x82,0x7c,0x00},//[)]
  {0x54,0x38,0xfe,0x38,0x54},//[*]
  {0x10,0x10,0x7c,0x10,0x10},//[+]
  {0x00,0x40,0xc0,0x00,0x00},//[,]
  {0x10,0x10,0x10,0x10,0x10},//[-]
  {0x00,0x00,0x40,0x00,0x00},//[.]
  {0x80,0x60,0x10,0x0c,0x02},//[/]
  {0x7c,0x82,0x92,0x82,0x7c},//[0]
  {0x00,0x82,0xfe,0x80,0x00},//[1]
  {0x84,0xc2,0xa2,0x92,0x8c},//[2]
  {0x44,0x82,0x92,0x92,0x6c},//[3]
  {0x30,0x2c,0x22,0xfe,0x20},//[4]
  {0xce,0x8a,0x8a,0x8a,0x72},//[5]
  {0x7c,0x92,0x92,0x92,0x62},//[6]
  {0x02,0xc2,0x22,0x12,0x0e},//[7]
  {0x6c,0x92,0x92,0x92,0x6c},//[8]
  {0x8c,0x92,0x92,0x92,0x7c},//[9]
  {0x00,0x00,0x44,0x00,0x00},//[:]
  {0x00,0x40,0xc4,0x00,0x00},//[;]
  {0x00,0x10,0x28,0x44,0x82},//[<]
  {0x44,0x44,0x44,0x44,0x44},//[=]
  {0x00,0x82,0x44,0x28,0x10},//[>]
  {0x0c,0x02,0xa2,0x12,0x0c},//[?]
  {0x7c,0x82,0xf2,0x92,0xfc},//[@]
  {0xf0,0x4c,0x42,0x4c,0xf0},//[A]
  {0xfe,0x92,0x92,0x92,0x6c},//[B]
  {0x7c,0x82,0x82,0x82,0x44},//[C]
  {0xfe,0x82,0x82,0x44,0x38},//[D]
  {0xfe,0x92,0x92,0x92,0x92},//[E]
  {0xfe,0x12,0x12,0x12,0x12},//[F]
  {0x7c,0x82,0xa2,0xa2,0x64},//[G]
  {0xfe,0x10,0x10,0x10,0xfe},//[H]
  {0x00,0x82,0xfe,0x82,0x00},//[I]
  {0x40,0x82,0x82,0x7e,0x02},//[J]
  {0xfe,0x10,0x28,0x44,0x82},//[K]
  {0xfe,0x80,0x80,0x80,0x80},//[L]
  {0xfe,0x08,0x30,0x08,0xfe},//[M]
  {0xfe,0x04,0x38,0x40,0xfe},//[N]
  {0x7c,0x82,0x82,0x82,0x7c},//[O]
  {0xfe,0x12,0x12,0x12,0x0c},//[P]
  {0x7c,0x82,0xa2,0x42,0xbc},//[Q]
  {0xfe,0x12,0x32,0x52,0x8c},//[R]
  {0x4c,0x92,0x92,0x92,0x64},//[S]
  {0x02,0x02,0xfe,0x02,0x02},//[T]
  {0x7e,0x80,0x80,0x80,0x7e},//[U]
  {0x0e,0x70,0x80,0x70,0x0e},//[V]
  {0xfe,0x40,0x38,0x40,0xfe},//[W]
  {0xc6,0x28,0x10,0x28,0xc6},//[X]
  {0x0e,0x10,0xe0,0x10,0x0e},//[Y]
  {0xc2,0xa2,0x92,0x8a,0x86},//[Z]
  {0x00,0xfe,0x82,0x82,0x00},//[[]
  {0x02,0x0c,0x10,0x60,0x80},//[\]
  {0x00,0x00,0x82,0x82,0xfe},//[]]
  {0x08,0x04,0x02,0x04,0x08},//[^]
  {0x80,0x80,0x80,0x80,0x80},//[_]
  {0x00,0x02,0x04,0x00,0x00},//[`]
  {0x60,0x94,0x94,0x94,0xf8},//[a]
  {0xfe,0x90,0x90,0x90,0x60},//[b]
  {0x00,0x70,0x88,0x88,0x88},//[c]
  {0x60,0x90,0x90,0xfe,0x00},//[d]
  {0x78,0x94,0x94,0x94,0x58},//[e]
  {0x00,0x90,0xfc,0x14,0x00},//[f]
  {0x40,0x8c,0x92,0x92,0xfe},//[g]
  {0x00,0xfe,0x10,0x10,0xe0},//[h]
  {0x00,0x00,0xfa,0x00,0x00},//[i]
  {0x40,0x80,0x80,0x7a,0x00},//[j]
  {0x00,0xfe,0x20,0x50,0x88},//[k]
  {0x00,0x02,0xfe,0x80,0x00},//[l]
  {0xf8,0x10,0xf0,0x10,0xf0},//[m]
  {0xf8,0x10,0x10,0x10,0xe0},//[n]
  {0x70,0x88,0x88,0x88,0x70},//[o]
  {0x00,0xf8,0x24,0x24,0x18},//[p]
  {0x00,0x18,0x24,0x24,0xf8},//[q]
  {0x00,0xf8,0x10,0x08,0x08},//[r]
  {0x00,0x48,0x94,0x94,0x60},//[s]
  {0x00,0x08,0xfc,0x88,0x00},//[t]
  {0x00,0x78,0x80,0x80,0xf8},//[u]
  {0x18,0x60,0x80,0x60,0x18},//[v]
  {0x38,0xc0,0x30,0xc0,0x38},//[w]
  {0x88,0x50,0x20,0x50,0x88},//[x]
  {0x8c,0x90,0xa0,0x60,0x1c},//[y]
  {0x88,0xc8,0xa8,0x98,0x88},//[z]
  {0x00,0x10,0xd6,0xaa,0x00},//[{]
  {0x00,0x00,0xfe,0x00,0x00},//[|]
  {0x00,0x00,0xaa,0xd6,0x10},//[}]
  {0x08,0x04,0x08,0x10,0x08} //[~]
};
