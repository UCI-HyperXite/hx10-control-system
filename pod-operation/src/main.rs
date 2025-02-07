mod components;

use tokio::sync::mpsc;
use tokio::task;

#[tokio::main]
async fn main() {
    let filename = components::storage::generate_filename();

    let (tx, mut rx) = mpsc::channel::<components::simulate::PodData>(100);

    let writer_task = task::spawn(async move {
        let mut cache: Vec<(f64, f64)> = Vec::new();

        while let Some(pod) = rx.recv().await {
            cache.push((pod.speed, pod.acceleration));

            if cache.len() >= 50 {
                components::storage::write_to_csv(&cache, &filename).await;
                cache.clear();
            }
        }
    });

    let simulation_task = task::spawn(async move {
        loop {
            let pod = components::simulate::run_simulation().await;
            println!(
                "Speed: {:.2} m/s, Acceleration: {:.2} m/s²",
                pod.speed, pod.acceleration
            );

            if tx.send(pod).await.is_err() {
                break;
            }
        }
    });

    let _ = tokio::join!(writer_task, simulation_task);
}
