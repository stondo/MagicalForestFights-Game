########################################################################################################################
# MagicalForestFights build stage
########################################################################################################################

FROM alpine:3.17.0 AS build

RUN apk update && \
    apk add --no-cache \
    build-base \
    cmake 

WORKDIR /MagicalForestFights

COPY includes/ ./includes/
COPY src/ ./src/
COPY test/ ./test/
COPY CMakeLists.txt .

WORKDIR /MagicalForestFights

RUN cmake -S . -B build && cmake --build build

########################################################################################################################
# MagicalForestFights image
########################################################################################################################

FROM alpine:3.17.0

RUN apk update && \
    apk add --no-cache \
    libstdc++

RUN addgroup -S shs && adduser -S shs -G shs
USER shs

COPY --chown=shs:shs --from=build \
    ./MagicalForestFights/build/MagicalForestFights_test \
    ./app/


COPY --chown=shs:shs --from=build \
    ./MagicalForestFights/build/MagicalForestFights \
    ./app/

RUN ./app/MagicalForestFights_test

ENTRYPOINT [ "./app/MagicalForestFights"]
