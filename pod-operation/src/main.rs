use axum::{routing::get, Router};
use socketioxide::SocketIo;

#[tokio::main]
async fn main() {
    // Make a layer for Socket connection
    let (layer, _io) = SocketIo::new_layer();

    // Initialize the
    let app: Router = Router::new()
        .route("/", get(|| async { "Hello world" }))
        .layer(layer);

    let listener = tokio::net::TcpListener::bind("0.0.0.0:5000").await.unwrap();

    axum::serve(listener, app).await.unwrap();
}
