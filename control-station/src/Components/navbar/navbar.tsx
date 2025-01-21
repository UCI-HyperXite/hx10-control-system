import "./navbar.css";
import HXLogo from "@/assets/images/HX-Logo.svg";

export default function Navbar() {
  return (
    <header className="navbar">
      <img src={HXLogo} alt="HyperXite Logo" />
      <h1>HyperXite</h1>
    </header>
  );
}
