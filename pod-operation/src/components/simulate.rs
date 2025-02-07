use rand::Rng;

#[derive(Debug, Clone)]
pub struct PodData {
    pub speed: f64,
    pub acceleration: f64,
}

impl PodData {
    pub fn new() -> Self {
        Self {
            speed: 0.0,
            acceleration: 0.0,
        }
    }

    pub fn update(&mut self) {
        let mut rng = rand::rng();
        self.acceleration = rng.random_range(-2.0..2.0);
        self.speed += self.acceleration;

        if self.speed < 0.0 {
            self.speed = 0.0;
        }
    }
}

pub async fn run_simulation() -> PodData {
    let mut pod_data = PodData::new();
    pod_data.update();
    pod_data
}
