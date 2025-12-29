def apply(config, args):
    if args.version == "kiosk":
        config["build_directory"] = "build"
        config["source_directories"] = ["src"]
    else:
        config["build_directory"] = f"build_{args.version}"
        config["source_directories"] = [f"src_{args.version}"]

    config["baseimg"] = f"baseroms/baserom.{args.version}.z64"
    config["myimg"] = f"{config["build_directory"]}/jfg.{args.version}.z64"
    config["mapfile"] = f"{config["build_directory"]}/jfg.{args.version}.map"
    config["show_line_numbers_default"] = True
    config["expected_dir"] = "expected/"

def add_custom_arguments(parser):
    parser.add_argument(
        "-v",
        "--VERSION",
        dest="version",
        default="us",
        type=str,
        help="Set the version",
    )
