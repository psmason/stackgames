#include <iostream>
#include <stdint.h>
#include <vector>

void fn(const std::string& prefix) {
  intptr_t rsp;
  asm ("mov %%rsp, %0" : "=r" (rsp));
  std::cout << prefix <<" :: fn address: " << std::hex << rsp << std::endl;
}

int main() {
  /* get the original stack pointer value */
  intptr_t rsp_copy;
  asm ("mov %%rsp, %0" : "=r" (rsp_copy));
  std::cout << "start address: " << std::hex << rsp_copy << std::endl;

  /* normal call on stack */
  fn("original");

  /* allocate new "stack" */
  std::vector<char> buffer;
  buffer.reserve(1024);
  std::cout << "buffer pointer: " << (void*) (buffer.data() + 1024) << std::endl;;
  

  /* assign new stack.  remember stack grows "down," so we use end of allocated region. */
  intptr_t new_rsp = reinterpret_cast<intptr_t>(buffer.data() + 1024);
  asm ("mov %0, %%rsp" : : "r" (new_rsp));
  
  fn("new");

  /* reset to original stack value */
  asm ("mov %0, %%rsp" : : "r" (rsp_copy));
}
