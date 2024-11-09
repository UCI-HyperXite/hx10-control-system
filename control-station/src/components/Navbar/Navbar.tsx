import HX from "../images/HX_Logo.svg";

import "./Navbar.css";

function Navbar() {
	return (
		<header className="navbar">
			<img alt="HX logo" src={HX} style={{ height: "60px" }} />
			HyperXite
		</header>
	);
}

export default Navbar;