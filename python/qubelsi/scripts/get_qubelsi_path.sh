#!/bin/bash

python << EOF
import qubelsi
print(qubelsi.__path__[0])
EOF
