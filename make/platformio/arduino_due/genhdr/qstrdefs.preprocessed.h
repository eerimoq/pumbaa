# 1 "<stdin>"
# 1 "/home/erik/workspace/pumbaa/examples/default-configuration//"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "<stdin>"
# 27 "<stdin>"
# 1 "/home/erik/workspace/pumbaa/micropython/py/mpconfig.h" 1
# 45 "/home/erik/workspace/pumbaa/micropython/py/mpconfig.h"
# 1 "/home/erik/workspace/pumbaa/src/port/mpconfigport.h" 1
# 34 "/home/erik/workspace/pumbaa/src/port/mpconfigport.h"
# 1 "/home/erik/workspace/pumbaa/src/boards/arduino_due/simba_board.h" 1
# 35 "/home/erik/workspace/pumbaa/src/port/mpconfigport.h" 2
# 1 "/home/erik/workspace/pumbaa/src/config.h" 1
# 35 "/home/erik/workspace/pumbaa/src/config.h"
# 1 "/home/erik/workspace/pumbaa/src/simba_config.h" 1
# 36 "/home/erik/workspace/pumbaa/src/config.h" 2
# 36 "/home/erik/workspace/pumbaa/src/port/mpconfigport.h" 2
# 1 "/home/erik/workspace/pumbaa/src/pumbaa_config_default.h" 1
# 422 "/home/erik/workspace/pumbaa/src/pumbaa_config_default.h"
extern const struct _mp_obj_module_t mp_module_uos;
extern const struct _mp_obj_module_t mp_module_utime;
extern const struct _mp_obj_module_t mp_module_usocket;
extern const struct _mp_obj_module_t mp_module_uselect;
extern const struct _mp_obj_module_t module_kernel;
extern const struct _mp_obj_module_t module_sync;
extern const struct _mp_obj_module_t module_drivers;
extern const struct _mp_obj_module_t module_inet;
extern const struct _mp_obj_module_t module_text;
extern const struct _mp_obj_module_t module_board;
# 538 "/home/erik/workspace/pumbaa/src/pumbaa_config_default.h"
typedef int mp_int_t;
typedef unsigned int mp_uint_t;
# 548 "/home/erik/workspace/pumbaa/src/pumbaa_config_default.h"
typedef long mp_off_t;





# 1 "/usr/include/newlib/alloca.h" 1 3
# 10 "/usr/include/newlib/alloca.h" 3
# 1 "/usr/include/newlib/_ansi.h" 1 3
# 15 "/usr/include/newlib/_ansi.h" 3
# 1 "/usr/include/newlib/newlib.h" 1 3
# 16 "/usr/include/newlib/_ansi.h" 2 3
# 1 "/usr/include/newlib/sys/config.h" 1 3



# 1 "/usr/include/newlib/machine/ieeefp.h" 1 3
# 5 "/usr/include/newlib/sys/config.h" 2 3
# 1 "/usr/include/newlib/sys/features.h" 1 3
# 6 "/usr/include/newlib/sys/config.h" 2 3
# 17 "/usr/include/newlib/_ansi.h" 2 3
# 11 "/usr/include/newlib/alloca.h" 2 3
# 1 "/usr/include/newlib/sys/reent.h" 1 3
# 13 "/usr/include/newlib/sys/reent.h" 3
# 1 "/usr/include/newlib/_ansi.h" 1 3
# 14 "/usr/include/newlib/sys/reent.h" 2 3
# 1 "/usr/lib/gcc/arm-none-eabi/4.9.3/include/stddef.h" 1 3 4
# 147 "/usr/lib/gcc/arm-none-eabi/4.9.3/include/stddef.h" 3 4
typedef int ptrdiff_t;
# 212 "/usr/lib/gcc/arm-none-eabi/4.9.3/include/stddef.h" 3 4
typedef unsigned int size_t;
# 324 "/usr/lib/gcc/arm-none-eabi/4.9.3/include/stddef.h" 3 4
typedef unsigned int wchar_t;
# 15 "/usr/include/newlib/sys/reent.h" 2 3
# 1 "/usr/include/newlib/sys/_types.h" 1 3
# 12 "/usr/include/newlib/sys/_types.h" 3
# 1 "/usr/include/newlib/machine/_types.h" 1 3






# 1 "/usr/include/newlib/machine/_default_types.h" 1 3
# 27 "/usr/include/newlib/machine/_default_types.h" 3
typedef signed char __int8_t;

typedef unsigned char __uint8_t;
# 41 "/usr/include/newlib/machine/_default_types.h" 3
typedef short int __int16_t;

typedef short unsigned int __uint16_t;
# 63 "/usr/include/newlib/machine/_default_types.h" 3
typedef long int __int32_t;

typedef long unsigned int __uint32_t;
# 89 "/usr/include/newlib/machine/_default_types.h" 3
typedef long long int __int64_t;

typedef long long unsigned int __uint64_t;
# 120 "/usr/include/newlib/machine/_default_types.h" 3
typedef signed char __int_least8_t;

typedef unsigned char __uint_least8_t;
# 146 "/usr/include/newlib/machine/_default_types.h" 3
typedef short int __int_least16_t;

typedef short unsigned int __uint_least16_t;
# 168 "/usr/include/newlib/machine/_default_types.h" 3
typedef long int __int_least32_t;

typedef long unsigned int __uint_least32_t;
# 186 "/usr/include/newlib/machine/_default_types.h" 3
typedef long long int __int_least64_t;

typedef long long unsigned int __uint_least64_t;
# 200 "/usr/include/newlib/machine/_default_types.h" 3
typedef int __intptr_t;

typedef unsigned int __uintptr_t;
# 8 "/usr/include/newlib/machine/_types.h" 2 3
# 13 "/usr/include/newlib/sys/_types.h" 2 3
# 1 "/usr/include/newlib/sys/lock.h" 1 3





typedef int _LOCK_T;
typedef int _LOCK_RECURSIVE_T;
# 14 "/usr/include/newlib/sys/_types.h" 2 3


typedef long _off_t;



typedef short __dev_t;



typedef unsigned short __uid_t;


typedef unsigned short __gid_t;



__extension__ typedef long long _off64_t;







typedef long _fpos_t;
# 55 "/usr/include/newlib/sys/_types.h" 3
typedef signed int _ssize_t;
# 67 "/usr/include/newlib/sys/_types.h" 3
# 1 "/usr/lib/gcc/arm-none-eabi/4.9.3/include/stddef.h" 1 3 4
# 353 "/usr/lib/gcc/arm-none-eabi/4.9.3/include/stddef.h" 3 4
typedef unsigned int wint_t;
# 68 "/usr/include/newlib/sys/_types.h" 2 3



typedef struct
{
  int __count;
  union
  {
    wint_t __wch;
    unsigned char __wchb[4];
  } __value;
} _mbstate_t;



typedef _LOCK_RECURSIVE_T _flock_t;




typedef void *_iconv_t;
# 16 "/usr/include/newlib/sys/reent.h" 2 3






typedef unsigned long __ULong;
# 38 "/usr/include/newlib/sys/reent.h" 3
struct _reent;






struct _Bigint
{
  struct _Bigint *_next;
  int _k, _maxwds, _sign, _wds;
  __ULong _x[1];
};


struct __tm
{
  int __tm_sec;
  int __tm_min;
  int __tm_hour;
  int __tm_mday;
  int __tm_mon;
  int __tm_year;
  int __tm_wday;
  int __tm_yday;
  int __tm_isdst;
};







struct _on_exit_args {
 void * _fnargs[32];
 void * _dso_handle[32];

 __ULong _fntypes;


 __ULong _is_cxa;
};
# 91 "/usr/include/newlib/sys/reent.h" 3
struct _atexit {
 struct _atexit *_next;
 int _ind;

 void (*_fns[32])(void);
        struct _on_exit_args _on_exit_args;
};
# 115 "/usr/include/newlib/sys/reent.h" 3
struct __sbuf {
 unsigned char *_base;
 int _size;
};
# 179 "/usr/include/newlib/sys/reent.h" 3
struct __sFILE {
  unsigned char *_p;
  int _r;
  int _w;
  short _flags;
  short _file;
  struct __sbuf _bf;
  int _lbfsize;






  void * _cookie;

  int (* _read) (struct _reent *, void *, char *, int)
                                          ;
  int (* _write) (struct _reent *, void *, const char *, int)

                                   ;
  _fpos_t (* _seek) (struct _reent *, void *, _fpos_t, int);
  int (* _close) (struct _reent *, void *);


  struct __sbuf _ub;
  unsigned char *_up;
  int _ur;


  unsigned char _ubuf[3];
  unsigned char _nbuf[1];


  struct __sbuf _lb;


  int _blksize;
  _off_t _offset;


  struct _reent *_data;



  _flock_t _lock;

  _mbstate_t _mbstate;
  int _flags2;
};
# 285 "/usr/include/newlib/sys/reent.h" 3
typedef struct __sFILE __FILE;



struct _glue
{
  struct _glue *_next;
  int _niobs;
  __FILE *_iobs;
};
# 317 "/usr/include/newlib/sys/reent.h" 3
struct _rand48 {
  unsigned short _seed[3];
  unsigned short _mult[3];
  unsigned short _add;




};
# 569 "/usr/include/newlib/sys/reent.h" 3
struct _reent
{
  int _errno;




  __FILE *_stdin, *_stdout, *_stderr;

  int _inc;
  char _emergency[25];

  int _current_category;
  const char *_current_locale;

  int __sdidinit;

  void (* __cleanup) (struct _reent *);


  struct _Bigint *_result;
  int _result_k;
  struct _Bigint *_p5s;
  struct _Bigint **_freelist;


  int _cvtlen;
  char *_cvtbuf;

  union
    {
      struct
        {
          unsigned int _unused_rand;
          char * _strtok_last;
          char _asctime_buf[26];
          struct __tm _localtime_buf;
          int _gamma_signgam;
          __extension__ unsigned long long _rand_next;
          struct _rand48 _r48;
          _mbstate_t _mblen_state;
          _mbstate_t _mbtowc_state;
          _mbstate_t _wctomb_state;
          char _l64a_buf[8];
          char _signal_buf[24];
          int _getdate_err;
          _mbstate_t _mbrlen_state;
          _mbstate_t _mbrtowc_state;
          _mbstate_t _mbsrtowcs_state;
          _mbstate_t _wcrtomb_state;
          _mbstate_t _wcsrtombs_state;
   int _h_errno;
        } _reent;



      struct
        {

          unsigned char * _nextf[30];
          unsigned int _nmalloc[30];
        } _unused;
    } _new;



  struct _atexit *_atexit;
  struct _atexit _atexit0;



  void (**(_sig_func))(int);




  struct _glue __sglue;
  __FILE __sf[3];
};
# 762 "/usr/include/newlib/sys/reent.h" 3
extern struct _reent *_impure_ptr ;
extern struct _reent *const _global_impure_ptr ;

void _reclaim_reent (struct _reent *);
# 12 "/usr/include/newlib/alloca.h" 2 3
# 555 "/home/erik/workspace/pumbaa/src/pumbaa_config_default.h" 2
# 37 "/home/erik/workspace/pumbaa/src/port/mpconfigport.h" 2
# 46 "/home/erik/workspace/pumbaa/micropython/py/mpconfig.h" 2
# 537 "/home/erik/workspace/pumbaa/micropython/py/mpconfig.h"
typedef float mp_float_t;
# 28 "<stdin>" 2





QCFG(BYTES_IN_LEN, (1))
QCFG(BYTES_IN_HASH, (2))

Q()
Q(*)
Q(_)
Q(%#o)
Q(%#x)
Q({:#b})
Q(\n)
Q(maximum recursion depth exceeded)
Q(<module>)
Q(<lambda>)
Q(<listcomp>)
Q(<dictcomp>)
Q(<setcomp>)
Q(<genexpr>)
Q(<string>)
Q(<stdin>)
Q(utf-8)


Q(__locals__)
Q(BufferError)
Q(FileExistsError)
Q(FileNotFoundError)
Q(FloatingPointError)
Q(UnboundLocalError)
Q(ADC_0)

Q(Adc)

Q(Adc)

Q(ArithmeticError)

Q(ArithmeticError)

Q(AssertionError)

Q(AssertionError)

Q(AssertionError)

Q(AttributeError)

Q(AttributeError)

Q(BOTH)

Q(BaseException)

Q(BaseException)

Q(BytesIO)

Q(BytesIO)

Q(CAN_0)

Q(CAN_1)

Q(Can)

Q(Can)

Q(DAC_0)

Q(Dac)

Q(Dac)

Q(DecompIO)

Q(DecompIO)

Q(Ds18b20)

Q(Ds18b20)

Q(EOFError)

Q(EOFError)

Q(EXTI_A0)

Q(EXTI_A1)

Q(EXTI_A10)

Q(EXTI_A11)

Q(EXTI_A2)

Q(EXTI_A3)

Q(EXTI_A4)

Q(EXTI_A5)

Q(EXTI_A6)

Q(EXTI_A7)

Q(EXTI_A8)

Q(EXTI_A9)

Q(EXTI_D0)

Q(EXTI_D1)

Q(EXTI_D10)

Q(EXTI_D11)

Q(EXTI_D12)

Q(EXTI_D13)

Q(EXTI_D14)

Q(EXTI_D15)

Q(EXTI_D16)

Q(EXTI_D17)

Q(EXTI_D18)

Q(EXTI_D19)

Q(EXTI_D2)

Q(EXTI_D20)

Q(EXTI_D21)

Q(EXTI_D22)

Q(EXTI_D23)

Q(EXTI_D24)

Q(EXTI_D25)

Q(EXTI_D26)

Q(EXTI_D27)

Q(EXTI_D28)

Q(EXTI_D29)

Q(EXTI_D3)

Q(EXTI_D30)

Q(EXTI_D31)

Q(EXTI_D32)

Q(EXTI_D33)

Q(EXTI_D34)

Q(EXTI_D35)

Q(EXTI_D36)

Q(EXTI_D37)

Q(EXTI_D38)

Q(EXTI_D39)

Q(EXTI_D4)

Q(EXTI_D40)

Q(EXTI_D41)

Q(EXTI_D42)

Q(EXTI_D43)

Q(EXTI_D44)

Q(EXTI_D45)

Q(EXTI_D46)

Q(EXTI_D47)

Q(EXTI_D48)

Q(EXTI_D49)

Q(EXTI_D5)

Q(EXTI_D50)

Q(EXTI_D51)

Q(EXTI_D52)

Q(EXTI_D53)

Q(EXTI_D6)

Q(EXTI_D7)

Q(EXTI_D8)

Q(EXTI_D9)

Q(EXTI_DAC0)

Q(EXTI_DAC1)

Q(EXTI_LED)

Q(Ellipsis)

Q(Ellipsis)

Q(Event)

Q(Event)

Q(Exception)

Q(Exception)

Q(Exti)

Q(Exti)

Q(FALLING)

Q(FLAGS_EXTENDED_FRAME)

Q(FLASH_0)

Q(GeneratorExit)

Q(GeneratorExit)

Q(INPUT)

Q(ImportError)

Q(ImportError)

Q(IndentationError)

Q(IndentationError)

Q(IndexError)

Q(IndexError)

Q(Input)

Q(KeyError)

Q(KeyError)

Q(KeyboardInterrupt)

Q(KeyboardInterrupt)

Q(LockType)

Q(LookupError)

Q(LookupError)

Q(MODE_MASTER)

Q(MODE_SLAVE)

Q(MemoryError)

Q(MemoryError)

Q(NameError)

Q(NameError)

Q(NoneType)

Q(NotImplementedError)

Q(NotImplementedError)

Q(OSError)

Q(OSError)

Q(OUTPUT)

Q(OrderedDict)

Q(OrderedDict)

Q(Output)

Q(OverflowError)

Q(OverflowError)

Q(Owi)

Q(Owi)

Q(PERIODIC)

Q(PIN_A0)

Q(PIN_A1)

Q(PIN_A10)

Q(PIN_A11)

Q(PIN_A2)

Q(PIN_A3)

Q(PIN_A4)

Q(PIN_A5)

Q(PIN_A6)

Q(PIN_A7)

Q(PIN_A8)

Q(PIN_A9)

Q(PIN_D0)

Q(PIN_D1)

Q(PIN_D10)

Q(PIN_D11)

Q(PIN_D12)

Q(PIN_D13)

Q(PIN_D14)

Q(PIN_D15)

Q(PIN_D16)

Q(PIN_D17)

Q(PIN_D18)

Q(PIN_D19)

Q(PIN_D2)

Q(PIN_D20)

Q(PIN_D21)

Q(PIN_D22)

Q(PIN_D23)

Q(PIN_D24)

Q(PIN_D25)

Q(PIN_D26)

Q(PIN_D27)

Q(PIN_D28)

Q(PIN_D29)

Q(PIN_D3)

Q(PIN_D30)

Q(PIN_D31)

Q(PIN_D32)

Q(PIN_D33)

Q(PIN_D34)

Q(PIN_D35)

Q(PIN_D36)

Q(PIN_D37)

Q(PIN_D38)

Q(PIN_D39)

Q(PIN_D4)

Q(PIN_D40)

Q(PIN_D41)

Q(PIN_D42)

Q(PIN_D43)

Q(PIN_D44)

Q(PIN_D45)

Q(PIN_D46)

Q(PIN_D47)

Q(PIN_D48)

Q(PIN_D49)

Q(PIN_D5)

Q(PIN_D50)

Q(PIN_D51)

Q(PIN_D52)

Q(PIN_D53)

Q(PIN_D6)

Q(PIN_D7)

Q(PIN_D8)

Q(PIN_D9)

Q(PIN_DAC0)

Q(PIN_DAC1)

Q(PIN_LED)

Q(POLLHUP)

Q(POLLIN)

Q(PWM_D10)

Q(PWM_D11)

Q(PWM_D12)

Q(PWM_D2)

Q(PWM_D3)

Q(PWM_D5)

Q(PWM_D6)

Q(PWM_D7)

Q(PWM_D8)

Q(PWM_D9)

Q(Pin)

Q(Pin)

Q(Queue)

Q(Queue)

Q(REFERENCE_VCC)

Q(RISING)

Q(RuntimeError)

Q(RuntimeError)

Q(SPEED_125KBPS)

Q(SPEED_1MBPS)

Q(SPEED_250KBPS)

Q(SPEED_2MBPS)

Q(SPEED_4MBPS)

Q(SPEED_500KBPS)

Q(SPEED_500KBPS)

Q(SPEED_8MBPS)

Q(SPI_0)

Q(Sd)

Q(Sd)

Q(Spi)

Q(Spi)

Q(StopAsyncIteration)

Q(StopAsyncIteration)

Q(StopAsyncIteration)

Q(StopIteration)

Q(StopIteration)

Q(StringIO)

Q(StringIO)

Q(SyntaxError)

Q(SyntaxError)

Q(SystemExit)

Q(SystemExit)

Q(TextIOWrapper)

Q(Timer)

Q(Timer)

Q(TypeError)

Q(TypeError)

Q(ValueError)

Q(ValueError)

Q(ZeroDivisionError)

Q(ZeroDivisionError)

Q(__add__)

Q(__aenter__)

Q(__aenter__)

Q(__aexit__)

Q(__aiter__)

Q(__anext__)

Q(__bool__)

Q(__build_class__)

Q(__call__)

Q(__class__)

Q(__class__)

Q(__class__)

Q(__class__)

Q(__class__)

Q(__class__)

Q(__class__)

Q(__contains__)

Q(__contains__)

Q(__del__)

Q(__del__)

Q(__del__)

Q(__delitem__)

Q(__delitem__)

Q(__dict__)

Q(__enter__)

Q(__enter__)

Q(__enter__)

Q(__enter__)

Q(__enter__)

Q(__enter__)

Q(__eq__)

Q(__eq__)

Q(__exit__)

Q(__exit__)

Q(__exit__)

Q(__exit__)

Q(__exit__)

Q(__exit__)

Q(__file__)

Q(__file__)

Q(__ge__)

Q(__getattr__)

Q(__getattr__)

Q(__getitem__)

Q(__getitem__)

Q(__getitem__)

Q(__getitem__)

Q(__gt__)

Q(__hash__)

Q(__import__)

Q(__init__)

Q(__init__)

Q(__init__)

Q(__init__)

Q(__init__)

Q(__init__)

Q(__init__)

Q(__init__)

Q(__init__)

Q(__init__)

Q(__iter__)

Q(__le__)

Q(__len__)

Q(__lt__)

Q(__main__)

Q(__main__)

Q(__main__)

Q(__main__)

Q(__main__)

Q(__main__)

Q(__main__)

Q(__module__)

Q(__name__)

Q(__name__)

Q(__name__)

Q(__name__)

Q(__name__)

Q(__name__)

Q(__name__)

Q(__name__)

Q(__name__)

Q(__name__)

Q(__name__)

Q(__name__)

Q(__name__)

Q(__name__)

Q(__name__)

Q(__name__)

Q(__name__)

Q(__name__)

Q(__name__)

Q(__name__)

Q(__name__)

Q(__name__)

Q(__name__)

Q(__name__)

Q(__name__)

Q(__name__)

Q(__name__)

Q(__name__)

Q(__name__)

Q(__name__)

Q(__name__)

Q(__name__)

Q(__name__)

Q(__new__)

Q(__new__)

Q(__new__)

Q(__next__)

Q(__next__)

Q(__next__)

Q(__next__)

Q(__path__)

Q(__path__)

Q(__path__)

Q(__qualname__)

Q(__repl_print__)

Q(__repl_print__)

Q(__repr__)

Q(__repr__)

Q(__reversed__)

Q(__setitem__)

Q(__setitem__)

Q(__str__)

Q(__sub__)

Q(__traceback__)

Q(_brace_open__colon__hash_b_brace_close_)

Q(_lt_dictcomp_gt_)

Q(_lt_genexpr_gt_)

Q(_lt_lambda_gt_)

Q(_lt_listcomp_gt_)

Q(_lt_module_gt_)

Q(_lt_setcomp_gt_)

Q(_lt_stdin_gt_)

Q(_lt_stdin_gt_)

Q(_lt_string_gt_)

Q(_percent__hash_o)

Q(_percent__hash_x)

Q(_star_)

Q(_star_)

Q(_thread)

Q(_thread)

Q(a2b_base64)

Q(abs)

Q(acos)

Q(acquire)

Q(add)

Q(all)

Q(allocate_lock)

Q(any)

Q(append)

Q(append)

Q(args)

Q(argv)

Q(array)

Q(array)

Q(array)

Q(array)

Q(asin)

Q(async_convert)

Q(async_convert)

Q(async_wait)

Q(async_wait)

Q(atan)

Q(atan2)

Q(b2a_base64)

Q(bin)

Q(binascii)

Q(board)

Q(board)

Q(bool)

Q(bool)

Q(bound_method)

Q(builtins)

Q(builtins)

Q(bytearray)

Q(bytearray)

Q(bytecode)

Q(byteorder)

Q(bytes)

Q(bytes)

Q(c_size)

Q(c_size)

Q(c_size_mult)

Q(calcsize)

Q(callable)

Q(callback)

Q(callback)

Q(ccc)

Q(ccc)

Q(ceil)

Q(channel)

Q(chdir)

Q(choice)

Q(chr)

Q(classmethod)

Q(classmethod)

Q(clear)

Q(clear)

Q(clear)

Q(close)

Q(close)

Q(close)

Q(close)

Q(close)

Q(close)

Q(close)

Q(closure)

Q(closure)

Q(cmath)

Q(cmath)

Q(collect)

Q(collections)

Q(complex)

Q(complex)

Q(const)

Q(const)

Q(convert)

Q(convert)

Q(convert)

Q(copy)

Q(copy)

Q(copy)

Q(copy)

Q(copy)

Q(copysign)

Q(cos)

Q(cos)

Q(count)

Q(count)

Q(count)

Q(crc)

Q(crc)

Q(crc)

Q(crc32)

Q(csd_structure)

Q(csd_structure)

Q(data)

Q(data)

Q(decode)

Q(decompress)

Q(default)

Q(degrees)

Q(delattr)

Q(deleter)

Q(deselect)

Q(device)

Q(device)

Q(device)

Q(device)

Q(device)

Q(devices)

Q(dict)

Q(dict)

Q(dict_view)

Q(difference)

Q(difference_update)

Q(digest)

Q(dir)

Q(disable)

Q(discard)

Q(divmod)

Q(doc)

Q(drivers)

Q(drivers)

Q(dsr_imp)

Q(dsr_imp)

Q(dumps)

Q(e)

Q(e)

Q(enable)

Q(encode)

Q(encoding)

Q(end)

Q(endswith)

Q(enumerate)

Q(enumerate)

Q(erase_blk_en)

Q(erase_blk_en)

Q(eval)

Q(event)

Q(exec)

Q(exit)

Q(exit)

Q(exp)

Q(exp)

Q(extend)

Q(extend)

Q(fabs)

Q(file)

Q(file)

Q(file_format)

Q(file_format)

Q(file_format_grp)

Q(file_format_grp)

Q(filter)

Q(filter)

Q(find)

Q(flags)

Q(flags)

Q(float)

Q(float)

Q(floor)

Q(flush)

Q(flush)

Q(fmod)

Q(format)

Q(format)

Q(frexp)

Q(from_bytes)

Q(fromkeys)

Q(frozenset)

Q(frozenset)

Q(function)

Q(function)

Q(function)

Q(function)

Q(function)

Q(function)

Q(function)

Q(function)

Q(function)

Q(gc)

Q(gc)

Q(generator)

Q(generator)

Q(generator)

Q(get)

Q(get_devices)

Q(get_devices)

Q(get_ident)

Q(get_temperature)

Q(getattr)

Q(getcwd)

Q(getrandbits)

Q(getter)

Q(getvalue)

Q(give_bus)

Q(globals)

Q(hasattr)

Q(hash)

Q(hashlib)

Q(heap_lock)

Q(heap_unlock)

Q(help)

Q(hex)

Q(hexlify)

Q(id)

Q(id)

Q(imag)

Q(implementation)

Q(index)

Q(index)

Q(index)

Q(inet)

Q(inet)

Q(input)

Q(insert)

Q(int)

Q(int)

Q(intersection)

Q(intersection_update)

Q(io)

Q(isalpha)

Q(isdigit)

Q(isdisjoint)

Q(isenabled)

Q(isfinite)

Q(isinf)

Q(isinstance)

Q(islower)

Q(isnan)

Q(isspace)

Q(issubclass)

Q(issubset)

Q(issuperset)

Q(isupper)

Q(items)

Q(iter)

Q(iterable)

Q(iterator)

Q(iterator)

Q(iterator)

Q(iterator)

Q(iterator)

Q(join)

Q(json)

Q(kernel)

Q(kernel)

Q(key)

Q(key)

Q(keys)

Q(keys)

Q(ldexp)

Q(len)

Q(list)

Q(list)

Q(listdir)

Q(little)

Q(little)

Q(little)

Q(load)

Q(loads)

Q(locals)

Q(localtime)

Q(lock)

Q(locked)

Q(log)

Q(log)

Q(lower)

Q(lstrip)

Q(machine)

Q(map)

Q(map)

Q(mask)

Q(math)

Q(math)

Q(max)

Q(mdt)

Q(mem_alloc)

Q(mem_current)

Q(mem_free)

Q(mem_info)

Q(mem_peak)

Q(mem_total)

Q(micropython)

Q(micropython)

Q(micropython)

Q(micropython)

Q(mid)

Q(min)

Q(mkdir)

Q(mode)

Q(mode)

Q(mode)

Q(modf)

Q(modify)

Q(module)

Q(modules)

Q(name)

Q(namedtuple)

Q(next)

Q(nodename)

Q(nsac)

Q(nsac)

Q(object)

Q(object)

Q(oct)

Q(oid)

Q(open)

Q(open)

Q(opt_level)

Q(ord)

Q(os)

Q(owi)

Q(pack)

Q(pack_into)

Q(path)

Q(perm_write_protect)

Q(perm_write_protect)

Q(phase)

Q(phase)

Q(pi)

Q(pi)

Q(pin_device)

Q(pin_device)

Q(ping_host_by_ip_address)

Q(platform)

Q(pnm)

Q(polar)

Q(polarity)

Q(poll)

Q(poll)

Q(poll)

Q(pop)

Q(pop)

Q(pop)

Q(popitem)

Q(pow)

Q(pow)

Q(print)

Q(print_exception)

Q(property)

Q(property)

Q(prv)

Q(psn)

Q(qstr_info)

Q(r)

Q(r2w_factor)

Q(r2w_factor)

Q(radians)

Q(randint)

Q(random)

Q(random)

Q(randrange)

Q(range)

Q(range)

Q(range)

Q(read)

Q(read)

Q(read)

Q(read)

Q(read)

Q(read)

Q(read)

Q(read)

Q(read)

Q(read)

Q(read_bl_len)

Q(read_bl_len)

Q(read_bl_partial)

Q(read_bl_partial)

Q(read_blk_misalign)

Q(read_blk_misalign)

Q(read_block)

Q(read_block_into)

Q(read_cid)

Q(read_csd)

Q(read_into)

Q(readinto)

Q(readinto)

Q(readinto)

Q(readinto)

Q(readline)

Q(readline)

Q(readline)

Q(readline)

Q(readlines)

Q(readlines)

Q(real)

Q(rect)

Q(reference)

Q(register)

Q(release)

Q(release)

Q(remove)

Q(remove)

Q(remove)

Q(rename)

Q(replace)

Q(repr)

Q(reset)

Q(reverse)

Q(reverse)

Q(reversed)

Q(reversed)

Q(rfind)

Q(rindex)

Q(rmdir)

Q(round)

Q(rsplit)

Q(rstrip)

Q(sample_rate)

Q(sampling_rate)

Q(search)

Q(sector_size)

Q(sector_size)

Q(seed)

Q(seek)

Q(seek)

Q(select)

Q(select)

Q(send)

Q(send)

Q(sep)

Q(set)

Q(set)

Q(set_mode)

Q(setattr)

Q(setdefault)

Q(setter)

Q(sha256)

Q(sha256)

Q(sin)

Q(sin)

Q(size)

Q(size)

Q(size)

Q(slave_select)

Q(sleep)

Q(sleep_ms)

Q(sleep_us)

Q(slice)

Q(slice)

Q(sort)

Q(sorted)

Q(speed)

Q(speed)

Q(spi)

Q(split)

Q(sqrt)

Q(sqrt)

Q(stack_size)

Q(start)

Q(start)

Q(start)

Q(start)

Q(start)

Q(start)

Q(start)

Q(start_new_thread)

Q(startswith)

Q(stat)

Q(staticmethod)

Q(staticmethod)

Q(stderr)

Q(stdin)

Q(stdout)

Q(step)

Q(stop)

Q(stop)

Q(stop)

Q(stop)

Q(stop)

Q(stop)

Q(str)

Q(str)

Q(strip)

Q(struct)

Q(sum)

Q(super)

Q(super)

Q(super)

Q(symmetric_difference)

Q(symmetric_difference_update)

Q(sync)

Q(sync)

Q(sys)

Q(sys)

Q(sys_lock)

Q(sys_reboot)

Q(sys_unlock)

Q(sysname)

Q(system)

Q(taac)

Q(taac)

Q(take_bus)

Q(tan)

Q(tell)

Q(text)

Q(text)

Q(thrd_get_by_name)

Q(thrd_get_env)

Q(thrd_get_global_env)

Q(thrd_get_log_mask)

Q(thrd_get_name)

Q(thrd_get_prio)

Q(thrd_join)

Q(thrd_self)

Q(thrd_set_env)

Q(thrd_set_global_env)

Q(thrd_set_log_mask)

Q(thrd_set_name)

Q(thrd_set_prio)

Q(thrd_yield)

Q(threshold)

Q(throw)

Q(throw)

Q(time)

Q(time)

Q(timeout)

Q(tm_hour)

Q(tm_isdst)

Q(tm_mday)

Q(tm_min)

Q(tm_mon)

Q(tm_sec)

Q(tm_wday)

Q(tm_yday)

Q(tm_year)

Q(tmp_write_protect)

Q(tmp_write_protect)

Q(to_bytes)

Q(toggle)

Q(tran_speed)

Q(tran_speed)

Q(transfer)

Q(transfer_into)

Q(trigger)

Q(trunc)

Q(tuple)

Q(tuple)

Q(tuple)

Q(type)

Q(type)

Q(ubinascii)

Q(ubinascii)

Q(ucollections)

Q(ucollections)

Q(uhashlib)

Q(uhashlib)

Q(uio)

Q(uio)

Q(ujson)

Q(ujson)

Q(uname)

Q(unhexlify)

Q(uniform)

Q(union)

Q(unpack)

Q(unpack_from)

Q(unregister)

Q(uos)

Q(uos)

Q(update)

Q(update)

Q(update)

Q(upper)

Q(urandom)

Q(urandom)

Q(uselect)

Q(uselect)

Q(ustruct)

Q(ustruct)

Q(utf_hyphen_8)

Q(utf_hyphen_8)

Q(utime)

Q(utime)

Q(uzlib)

Q(uzlib)

Q(value)

Q(values)

Q(vdd_r_curr_max)

Q(vdd_r_curr_min)

Q(vdd_w_curr_max)

Q(vdd_w_curr_min)

Q(version)

Q(version)

Q(version)

Q(version_info)

Q(wp_grp_enable)

Q(wp_grp_enable)

Q(wp_grp_size)

Q(wp_grp_size)

Q(write)

Q(write)

Q(write)

Q(write)

Q(write)

Q(write)

Q(write)

Q(write)

Q(write)

Q(write_bl_len)

Q(write_bl_len)

Q(write_bl_partial)

Q(write_blk_misalign)

Q(write_blk_misalign)

Q(write_block)

Q(write_partial)

Q(zip)

Q(zip)

Q(zlib)
