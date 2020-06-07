docker run -ti -v $(pwd):$(pwd) -u $(id -u):$(id -g) -w $(pwd) lights_control:latest make -C test test
