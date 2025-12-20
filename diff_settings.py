def apply(config, args):
    config["baseimg"] = "baseroms/baserom.us.z64"
    config["myimg"] = "build/jfg.us.z64"
    config["mapfile"] = "build/jfg.us.map"
    config["source_directories"] = ["src"]
    config["show_line_numbers_default"] = True
    config["expected_dir"] = "expected/"
