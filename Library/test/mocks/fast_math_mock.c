#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include "cmock.h"
#include "fast_math_mock.h"


static struct fast_math_mockInstance
{
  unsigned char placeHolder;
} Mock;

extern jmp_buf AbortFrame;
extern int GlobalExpectCount;
extern int GlobalVerifyOrder;

void fast_math_mock_Verify(void)
{
}

void fast_math_mock_Init(void)
{
  fast_math_mock_Destroy();
}

void fast_math_mock_Destroy(void)
{
  CMock_Guts_MemFreeAll();
  memset(&Mock, 0, sizeof(Mock));
  GlobalExpectCount = 0;
  GlobalVerifyOrder = 0;
}

