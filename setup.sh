#!/bin/bash
git submodule init
git submodule update
docker build ./docker -t lights_control
