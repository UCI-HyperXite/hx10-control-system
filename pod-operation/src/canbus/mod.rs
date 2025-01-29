pub mod bms;
pub mod vfd;
pub mod imd;
pub mod utils;

pub enum CanDevice {
    Bms,
    Vfd,
    Imd,
}

pub fn init_canbus() {
    println!("Initializing CANbus");
}