def apply(config, args):
    config["baseimg"] = "baseroms/baserom.kiosk.z64"
    config["myimg"] = "build/jfg.kiosk.z64"
    config["mapfile"] = "build/jfg.kiosk.map"
    config["source_directories"] = ["src"]
    config["show_line_numbers_default"] = True
    config["expected_dir"] = "expected/"
