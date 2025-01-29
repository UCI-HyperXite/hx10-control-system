pub fn decode_bms_data(data: &[u8]) -> (f32, f32, u16) {
    // decode pack current, pack voltage, and diagnostic flags
    let pack_current = f32::from(data[0]) * 0.1;
    let pack_voltage = f32::from(data[1]) * 0.1;
    let dtc_flags = u16::from(data[2]);
    (pack_current, pack_voltage, dtc_flags)
}

pub fn decode_bms_temp(data: &[u8]) -> (f32, f32, f32) {
    // decode temperature, voltage
    let temp_avg = f32::from(data[0]) * 0.1;
    let temp_low = f32::from(data[1]) * 0.1;
    let high_cell_voltage = f32::from(data[2]) * 0.001;
    (temp_avg, temp_low, high_cell_voltage)
}
