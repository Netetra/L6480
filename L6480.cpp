#include "L6480.h"


enum class L6480::Register: uint8_t {
  ABS_POS    = 0x01,
  EL_POS     = 0x02,
  MARK       = 0x03,
  SPEED      = 0x04,
  ACC        = 0x05,
  DEC_       = 0x06,
  MAX_SPEED  = 0x07,
  MIN_SPEED  = 0x08,
  KVAL_HOLD  = 0x09,
  KVAL_RUN   = 0x0A,
  KVAL_ACC   = 0x0B,
  KVAL_DEC   = 0x0C,
  INT_SPEED  = 0x0D,
  ST_SLP     = 0x0E,
  FN_SLP_ACC = 0x0F,
  FN_SLP_DEC = 0x10,
  K_THERM    = 0x11,
  ADC_OUT    = 0x12,
  OCD_TH     = 0x13,
  STALL_TH   = 0x14,
  FS_SPD     = 0x15,
  STEP_MODE  = 0x16,
  ALARM_EN   = 0x17,
  GATECFG1   = 0x18,
  GATECFG2   = 0x19,
  CONFIG     = 0x1A,
  STATUS     = 0x1B,
};


L6480::L6480(int busy_pin, int ss_pin) {
  this->busy_pin = busy_pin;
  this->ss_pin = ss_pin;
}


void L6480::begin() {
  pinMode(this->ss_pin, OUTPUT);
  digitalWrite(this->ss_pin, HIGH);

  for (int i = 0; i < 5; i++) {
    this->nop();
  }
}


void L6480::wait_busy() {
  while (!digitalRead(this->busy_pin)) {}
}


void L6480::send(uint8_t data) {
  this->wait_busy();
  digitalWrite(this->ss_pin, LOW);
  SPI.transfer(data);
  digitalWrite(this->ss_pin, HIGH);
}


void L6480::send_u(uint8_t data) {
  digitalWrite(this->ss_pin, LOW);
  SPI.transfer(data);
  digitalWrite(this->ss_pin, HIGH);
}


void L6480::transfer(uint8_t op, uint8_t bytes, long value) {
  int data[3];
  this->send(op);

  for (int i = 0; i < bytes; i++) {
    data[i] = value & 0xff;
    value = value >> 8;
  }

  if (bytes == 3) {
    this->send(data[2]);
  }
  if (bytes >= 2) {
    this->send(data[1]);
  }
  if (bytes >= 1) {
    this->send(data[0]);
  }
}


long L6480::get_param(Register reg, uint8_t bytes) {
  uint8_t address = static_cast<uint8_t>(reg);
  int send_cmd = address | 0x20;
  this->send_u(send_cmd);
  long value = 0;

  for (uint8_t i = 0; i < bytes; i++) {
    value = value << 8;
    digitalWrite(this->ss_pin, LOW);
    value = value | SPI.transfer(0x00);
    digitalWrite(this->ss_pin, HIGH);
  }

  return value;
}


long L6480::set_param(Register reg, uint8_t bytes, long value) {
  uint8_t address = static_cast<uint8_t>(reg);
  this->transfer(address, bytes, value);
}


void L6480::nop() {
  this->transfer(0x00, 0, 0);
}


void L6480::set_abs_pos(long value) {
  this->set_param(Register::ABS_POS, 3, value);
}


void L6480::set_el_pos(long value) {
  this->set_param(Register::EL_POS, 2, value);
}


void L6480::set_mark(long value) {
  this->set_param(Register::MARK, 3, value);
}


// SPEEDは読み込み専用なので無い //


void L6480::set_acc(long value) {
  this->set_param(Register::ACC, 2, value);
}


void L6480::set_dec(long value) {
  this->set_param(Register::DEC_, 2, value);
}


void L6480::set_max_speed(long value) {
  this->set_param(Register::MAX_SPEED, 2, value);
}


void L6480::set_min_speed(long value) {
  this->set_param(Register::MIN_SPEED, 2, value);
}


void L6480::set_kval_hold(long value) {
  this->set_param(Register::KVAL_HOLD, 1, value);
}


void L6480::set_kval_run(long value) {
  this->set_param(Register::KVAL_RUN, 1, value);
}


void L6480::set_kval_acc(long value) {
  this->set_param(Register::KVAL_ACC, 1, value);
}


void L6480::set_kval_dec(long value) {
  this->set_param(Register::KVAL_DEC, 1, value);
}


void L6480::set_int_speed(long value) {
  this->set_param(Register::INT_SPEED, 2, value);
}


void L6480::set_st_slp(long value) {
  this->set_param(Register::ST_SLP, 1, value);
}


void L6480::set_fn_slp_acc(long value) {
  this->set_param(Register::FN_SLP_ACC, 1, value);
}


void L6480::set_fn_slp_dec(long value) {
  this->set_param(Register::FN_SLP_DEC, 1, value);
}


void L6480::set_k_therm(long value) {
  this->set_param(Register::K_THERM, 1, value);
}


// ADC_OUTは読み込み専用なので無い //


void L6480::set_ocd_th(long value) {
  this->set_param(Register::OCD_TH, 1, value);
}


void L6480::set_stall_th(long value) {
  this->set_param(Register::STALL_TH, 1, value);
}


void L6480::set_fs_spd(long value) {
  this->set_param(Register::FS_SPD, 2, value);
}


void L6480::set_step_mode(long value) {
  this->set_param(Register::STEP_MODE, 1, value);
}


void L6480::set_alarm_en(long value) {
  this->set_param(Register::ALARM_EN, 1, value);
}


void L6480::set_gatecfg1(long value) {
  this->set_param(Register::GATECFG1, 2, value);
}


void L6480::set_gatecfg2(long value) {
  this->set_param(Register::GATECFG2, 1, value);
}


void L6480::set_config(long value) {
  this->set_param(Register::CONFIG, 2, value);
}


// STATUSは読み込み専用なので無い //


long L6480::get_abs_pos() {
  return this->get_param(Register::ABS_POS, 3);
}


long L6480::get_el_pos() {
  return this->get_param(Register::EL_POS, 2);
}


long L6480::get_mark() {
  return this->get_param(Register::MARK, 3);
}


long L6480::get_speed() {
  return this->get_param(Register::SPEED, 3);
}


long L6480::get_acc() {
  return this->get_param(Register::ACC, 2);
}


long L6480::get_dec() {
  return this->get_param(Register::DEC_, 2);
}


long L6480::get_max_speed() {
  return this->get_param(Register::MAX_SPEED, 2);
}


long L6480::get_min_speed() {
  return this->get_param(Register::MIN_SPEED, 2);
}


long L6480::get_kval_hold() {
  return this->get_param(Register::KVAL_HOLD, 1);
}


long L6480::get_kval_run() {
  return this->get_param(Register::KVAL_RUN, 1);
}


long L6480::get_kval_acc() {
  return this->get_param(Register::KVAL_ACC, 1);
}


long L6480::get_kval_dec() {
  return this->get_param(Register::KVAL_DEC, 1);
}


long L6480::get_int_speed() {
  return this->get_param(Register::INT_SPEED, 2);
}


long L6480::get_st_slp() {
  return this->get_param(Register::ST_SLP, 1);
}


long L6480::get_fn_slp_acc() {
  return this->get_param(Register::FN_SLP_ACC, 1);
}


long L6480::get_fn_slp_dec() {
  return this->get_param(Register::FN_SLP_DEC, 1);
}


long L6480::get_k_therm() {
  return this->get_param(Register::K_THERM, 1);
}


long L6480::get_adc_out() {
  return this->get_param(Register::ADC_OUT, 1);
}


long L6480::get_ocd_th() {
  return this->get_param(Register::OCD_TH, 1);
}


long L6480::get_stall_th() {
  return this->get_param(Register::STALL_TH, 1);
}


long L6480::get_fs_spd() {
  return this->get_param(Register::FS_SPD, 2);
}


long L6480::get_step_mode() {
  return this->get_param(Register::STEP_MODE, 1);
}


long L6480::get_alarm_en() {
  return this->get_param(Register::ALARM_EN, 1);
}


void L6480::get_gatecfg1() {
  return this->get_param(Register::GATECFG1, 2);
}


void L6480::get_gatecfg2() {
  return this->get_param(Register::GATECFG2, 1);
}


long L6480::get_config() {
  return this->get_param(Register::CONFIG, 2);
}


long L6480::get_status() {
  return this->get_param(Register::STATUS, 2);
}


void L6480::run(Dir dir, long speed) {
  if (dir == Dir::CW) {
    this->transfer(0x50, 3, speed);
  }
  else if (dir == Dir::CCW) {
    this->transfer(0x51, 3, speed);
  }
}


void L6480::step_clock(Dir dir) {
  if (dir == Dir::CW) {
    this->transfer(0x58, 0, 0);
  }
  else if (dir == Dir::CCW) {
    this->transfer(0x59, 0, 0);
  }
}


void L6480::move(Dir dir, long n_step) {
  if (dir == Dir::CW) {
    this->transfer(0x40, 3, n_step);
  }
  else if (dir == Dir::CCW) {
    this->transfer(0x41, 3, n_step);
  }
}


void L6480::goto_(long pos) {
  this->transfer(0x60, 3, pos);
}


void L6480::goto_dir(Dir dir, long pos) {
  if (dir == Dir::CW) {
    this->transfer(0x68, 3, pos);
  }
  else if (dir == Dir::CCW) {
    this->transfer(0x69, 3, pos);
  }
}


void L6480::go_until(bool act, Dir dir, long speed) {
  if (act) {
    if (dir == Dir::CW) {
      this->transfer(0x8A, 3, speed);
    }
    else if (dir == Dir::CCW) {
      this->transfer(0x8B, 3, speed);
    }
  }
  else {
    if (dir == Dir::CW) {
      this->transfer(0x82, 3, speed);
    }
    else if (dir == Dir::CCW) {
      this->transfer(0x83, 3, speed);
    }
  }
}


void L6480::relese_sw(int act, Dir dir) {
  if (act) {
    if (dir == Dir::CW) {
      this->transfer(0x9A, 0, 0);
    }
    else if (dir == Dir::CCW) {
      this->transfer(0x9B, 0, 0);
    }
  }
  else {
    if (dir == Dir::CW) {
      this->transfer(0x92, 0, 0);
    }
    else if (dir == Dir::CCW) {
      this->transfer(0x93, 0, 0);
    }
  }
}


void L6480::go_home() {
  this->transfer(0x70, 0, 0);
}


void L6480::go_mark() {
  this->transfer(0x78, 0, 0);
}


void L6480::reset_pos() {
  this->transfer(0xD8, 0, 0);
}


void L6480::reset_device() {
  this->transfer(0xC0, 0, 0);
}


void L6480::soft_stop() {
  this->transfer(0xB0, 0, 0);
}


void L6480::hard_stop() {
  this->transfer(0xB8, 0, 0);
}


void L6480::soft_hiz() {
  this->transfer(0xA0, 0, 0);
}


void L6480::hard_hiz() {
  this->transfer(0xA8, 0, 0);
}


/*
long L6480::get_status2() {
  this->send_u(0xd0);
  long value = 0;
  for (uint8_t i = 0; i<=1; i++) {
    value = value << 8;
    digitalWrite(this->ss_pin, LOW);
    value = value | SPI.transfer(0x00);
    digitalWrite(this->ss_pin, HIGH);
  }
  return value;
}
*/
