########################################################################################################################
# MagicalForestFights build stage
########################################################################################################################

FROM ubuntu:latest AS build

RUN apt-get update && \
    apt-get install -y \
    build-essential \
    cmake \
    git 


WORKDIR /MagicalForestFights

RUN git clone --recursive https://github.com/biojppm/rapidyaml
RUN cd rapidyaml && mkdir build && cd build && cmake .. && make install

# RUN git clone https://github.com/gabime/spdlog.git
# RUN cd spdlog && mkdir build && cd build && cmake .. && make -j && make install

COPY lib/spdlog ./lib/spdlog
COPY includes/ ./includes/
COPY src/ ./src/
COPY test/ ./test/
COPY CMakeLists.txt .

WORKDIR /MagicalForestFights

RUN cmake -S . -B build && cmake --build build

########################################################################################################################
# MagicalForestFights image
########################################################################################################################

FROM ubuntu:latest

RUN apt-get update && \
    apt-get install -y \
    libstdc++6

#RUN addgroup shs
RUN adduser shs
USER shs

COPY --chown=shs:shs --from=build \
    ./MagicalForestFights/build/MagicalForestFights_test \
    ./app/


COPY --chown=shs:shs --from=build \
    ./MagicalForestFights/build/MagicalForestFights \
    ./app/

RUN ./app/MagicalForestFights_test

ENTRYPOINT [ "./app/MagicalForestFights"]
