use std::fs::OpenOptions;
use csv::Writer;
use chrono::Local;

pub fn generate_filename() -> String {
    Local::now().format("%Y-%m-%d_%H-%M-%S.csv").to_string()
}

pub async fn write_to_csv(data: &Vec<(f64, f64)>, filename: &str) {
    let file = OpenOptions::new()
        .write(true)
        .append(true)
        .create(true)
        .open(filename)
        .expect("Failed to open file");

    let mut wtr = Writer::from_writer(file);

    if std::fs::metadata(filename).map(|m| m.len()).unwrap_or(0) == 0 {
        wtr.write_record(&["Speed (m/s)", "Acceleration (m/s²)"]).unwrap();
    }

    for &(speed, acceleration) in data {
        wtr.write_record(&[speed.to_string(), acceleration.to_string()])
            .unwrap();
    }

    wtr.flush().unwrap();
}
