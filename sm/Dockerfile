FROM alpine:latest

ENV DEBIAN_FRONTEND noninteractive

ADD install.sh install.sh
RUN sh install.sh && rm install.sh
RUN apt-get install python-pip

VOLUME ["/source"]
WORKDIR /source
CMD ["bash"]

