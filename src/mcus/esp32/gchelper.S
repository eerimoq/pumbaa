    .file   "gchelper.s"
    .text

    .align  4
    .global gc_helper_get_regs_and_sp
    .type   gc_helper_get_regs_and_sp, @function
gc_helper_get_regs_and_sp:
    entry a1, 16

    mov     a2, a1
    retw.n

    .size   gc_helper_get_regs_and_sp, .-gc_helper_get_regs_and_sp
