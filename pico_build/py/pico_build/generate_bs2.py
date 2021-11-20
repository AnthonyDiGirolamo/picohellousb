#!/usr/bin/env python3
# Copyright 2021 The Pigweed Authors
#
# Licensed under the Apache License, Version 2.0 (the "License"); you may not
# use this file except in compliance with the License. You may obtain a copy of
# the License at
#
#     https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
# License for the specific language governing permissions and limitations under
# the License.
"""PNG to rgb565 header file converter."""

import argparse
import logging
from pathlib import Path

from jinja2 import Environment, FileSystemLoader, make_logging_undefined

jinja_env = Environment(
    # Load templates automatically from pw_console/templates
    loader=FileSystemLoader(Path(__file__).parent / 'templates'),
    # Raise errors if variables are undefined in templates
    undefined=make_logging_undefined(logger=logging.getLogger(__package__), ),
    # Trim whitespace in templates
    trim_blocks=True,
    lstrip_blocks=True,
)


def _arg_parser():
    """Setup argparse."""
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('OUTPUT_PATH', type=Path, help="output_path")
    return parser.parse_args()


def main() -> None:
    args = _arg_parser()

    output_path = args.output_path
    if not output_path.is_dir():
        raise FileNotFoundError

    template = jinja_env.get_template('bs2.jinja')
    output_file = output_path / 'bs2_generated.h'
    output_file.write_text(template.render(text="hello"))


if __name__ == '__main__':
    main()
