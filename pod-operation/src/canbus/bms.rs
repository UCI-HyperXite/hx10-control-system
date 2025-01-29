use socketcan::{CanFrame, CanSocket};
use std::collections::HashMap;
use std::time::Duration;

// Enum for BMS states
// class Bms(Enum):
// VOLTAGE_IN_RANGE = 1
// LOW_CELL_VOLTAGE = 2
enum BmsState {
    VoltageInRange,
    LowCellVoltage,
}

fn decode(can_id: u32, data: &[u8]) -> Result<HashMap<String, f32>, String> {
    // message encodings from original Python script 
    match can_id {
        0x101 => {
            if data.len() >= 7 {
                let temp_avg = f32::from(u16::from_le_bytes([data[0], data[1]])) * 0.1;
                let temp_low = f32::from(u16::from_le_bytes([data[2], data[3]])) * 0.1;
                let high_cell_voltage = f32::from(u16::from_be_bytes([data[5], data[6]])) * 0.0001;
                let mut result = HashMap::new();
                result.insert("temp_avg".to_string(), temp_avg);
                result.insert("temp_low".to_string(), temp_low);
                result.insert("high_cell_voltage".to_string(), high_cell_voltage);
                Ok(result)
            } else {
                Err("Insufficient data for ID 101".to_string())
            }
        }
        0x100 => {
            if data.len() >= 7 {
                let pack_current = f32::from(u16::from_be_bytes([data[0], data[1]])) * 0.1;
                let pack_voltage = f32::from(u16::from_le_bytes([data[2], data[3]])) * 0.1;
                let temp_high = f32::from(data[4]);
                let dtc_flags = u16::from_le_bytes([data[5], data[6]]);
                let mut result = HashMap::new();
                result.insert("pack_current".to_string(), pack_current);
                result.insert("pack_voltage".to_string(), pack_voltage);
                result.insert("temp_high".to_string(), temp_high);
                result.insert("dtc_flags".to_string(), f32::from(dtc_flags));
                Ok(result)
            } else {
                Err("Insufficient data for ID 100".to_string())
            }
        }
        0x103 => {
            if data.len() >= 7 {
                let cell_id = data[0] as usize + 1;
                let instant_voltage = f32::from(u16::from_be_bytes([data[1], data[2]])) * 0.1;
                let internal_resistance = f32::from(u16::from_be_bytes([data[3], data[4]])) * 0.01;
                let open_voltage = f32::from(u16::from_be_bytes([data[5], data[6]])) * 0.1;
                let mut result = HashMap::new();
                result.insert("cell_id".to_string(), cell_id as f32);
                result.insert("instant_voltage".to_string(), instant_voltage);
                result.insert("internal_resistance".to_string(), internal_resistance);
                result.insert("open_voltage".to_string(), open_voltage);
                Ok(result)
            } else {
                Err("Insufficient data for ID 103".to_string())
            }
        }
        _ => Err("Unknown CAN ID".to_string()),
    }
}

// Function to monitor BMS and check low voltage
fn low_voltage_check() {
    // open CAN socket
    let can_socket = CanSocket::open("can0").expect("CAN socket did not open aaaaAAAAAA");
    can_socket.set_read_timeout(Duration::from_secs(1)).unwrap();

    let mut cells = vec![0.0; 12];
    let mut low_voltage_detected = false;

    // read_frame() method listens for incoming frames 
    while !low_voltage_detected {
        match can_socket.read_frame() { 
            Ok(frame) => {
                let can_id = frame.id();
                let data = frame.data();

                match decode(can_id, data) {
                    Ok(decoded_data) => {
                        if can_id == 0x103 {
                            let cell_id = decoded_data["cell_id"] as usize;
                            if cell_id <= 12 {
                                cells[cell_id - 1] = decoded_data["instant_voltage"];
                            }
                        } else if can_id == 0x100 {
                            if let Some(pack_voltage) = decoded_data.get("pack_voltage") {
                                println!("Pack Voltage: {:.2} V", pack_voltage);
                            }
                        } else if can_id == 0x101 {
                            if let Some(temp_avg) = decoded_data.get("temp_avg") {
                                println!("Average Temperature: {:.1} C", temp_avg);
                            }
                        }
                    }
                    Err(e) => {
                        println!("Error decoding CAN message: {}", e);
                    }
                }

                for (index, &voltage) in cells.iter().enumerate() {
                    if voltage > 0.0 && voltage <= 3400.0 {
                        println!(
                            "ERROR: {:?}\nCell: {} Voltage: {:.2} mV",
                            BmsState::LowCellVoltage,
                            index + 1,
                            voltage
                        );
                        low_voltage_detected = true;
                        break;
                    } else {
                        println!("Cell: {} Voltage: {:.2} mV", index + 1, voltage);
                    }
                }
            }
            Err(e) => {
                println!("Error reading CAN frame: {}", e);
            }
        }
    }
}

fn main() {
    low_voltage_check();
}
