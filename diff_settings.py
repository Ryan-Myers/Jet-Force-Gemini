def apply(config, args):
    if args.VERSION == "us" or args.VERSION == "kiosk":
        config["build_directory"] = "build"
        config["source_directories"] = ["src"]
    else:
        config["build_directory"] = f"build_{args.VERSION}"
        config["source_directories"] = [f"src_{args.VERSION}"]

    config["baseimg"] = f"baseroms/baserom.{args.VERSION}.z64"
    config["myimg"] = f"{config['build_directory']}/jfg.{args.VERSION}.z64"
    config["mapfile"] = f"{config['build_directory']}/jfg.{args.VERSION}.map"
    config["show_line_numbers_default"] = True
    config["expected_dir"] = "expected/"

def add_custom_arguments(parser):
    parser.add_argument(
        "-v",
        "--VERSION",
        dest="VERSION",
        default="us",
        type=str,
        help="Set the version",
    )
