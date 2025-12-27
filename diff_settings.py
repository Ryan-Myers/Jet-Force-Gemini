def apply(config, args):
    config["baseimg"] = "baseroms/baserom.us.z64"
    config["myimg"] = "build_us/jfg.us.z64"
    config["mapfile"] = "build_us/jfg.us.map"
    config["source_directories"] = ["src_us"]
    config["show_line_numbers_default"] = True
    config["expected_dir"] = "expected/"
