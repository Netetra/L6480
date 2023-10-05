#include <SPI.h>

#ifndef _L6480_HEAD_
#define _L6480_HEAD_

#if ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif


class L6480 {
  public:
    enum class Dir: bool { CW, CCW, };
    L6480(int busy_pin, int ss_pin);
    void begin();
    void wait_busy();

    void nop();
    void set_abs_pos(long value);
    void set_el_pos(long value);
    void set_mark(long value);
    void set_acc(long value);
    void set_dec(long value);
    void set_max_speed(long value);
    void set_min_speed(long value);
    void set_kval_hold(long value);
    void set_kval_run(long value);
    void set_kval_acc(long value);
    void set_kval_dec(long value);
    void set_int_speed(long value);
    void set_st_slp(long value);
    void set_fn_slp_acc(long value);
    void set_fn_slp_dec(long value);
    void set_k_therm(long value);
    void set_ocd_th(long value);
    void set_stall_th(long value);
    void set_fs_spd(long value);
    void set_step_mode(long value);
    void set_alarm_en(long value);
    void set_gatecfg1(long value);
    void set_gatecfg2(long value);
    void set_config(long value);
    long get_abs_pos();
    long get_el_pos();
    long get_mark();
    long get_speed();
    long get_acc();
    long get_dec();
    long get_max_speed();
    long get_min_speed();
    long get_kval_hold();
    long get_kval_run();
    long get_kval_acc();
    long get_kval_dec();
    long get_int_speed();
    long get_st_slp();
    long get_fn_slp_acc();
    long get_fn_slp_dec();
    long get_k_therm();
    long get_adc_out();
    long get_ocd_th();
    long get_stall_th();
    long get_fs_spd();
    long get_step_mode();
    long get_alarm_en();
    void get_gatecfg1();
    void get_gatecfg2();
    long get_config();
    long get_status();
    void run(Dir dir, long speed);
    void step_clock(Dir dir);
    void move(Dir dir, long n_step);
    void goto_(long pos);
    void goto_dir(Dir dir, long pos);
    void go_until(bool act, Dir dir, long speed);
    void relese_sw(int act, Dir dir);
    void go_home();
    void go_mark();
    void reset_pos();
    void reset_device();
    void soft_stop();
    void hard_stop();
    void soft_hiz();
    void hard_hiz();

  private:
    int ss_pin;
    int busy_pin;
    enum class Register: uint8_t;
    void send(uint8_t data);
    void send_u(uint8_t data);
    void transfer(uint8_t op, uint8_t bytes, long value);
    long get_param(Register reg, uint8_t bytes);
    long set_param(Register reg, uint8_t bytes, long value);
};

#endif
