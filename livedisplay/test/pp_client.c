/*
** Copyright 2016, The CyanogenMod Project
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "cutils/sockets.h"

#define BUF_SZ 4096

static void send_pp_cmd(char *buf, size_t len)
{
    int sock = socket_local_client("pps",
                                   ANDROID_SOCKET_NAMESPACE_RESERVED,
                                   SOCK_STREAM);
    if (sock >= 0) {
        if (write(sock, buf, strlen(buf) + 1) > 0) {
            memset(buf, 0, len);
            ssize_t ret;
            while ((ret = read(sock, buf, len)) > 0) {
                if ((size_t)ret == len) {
                    break;
                }
                len -= ret;
                buf += ret;

                struct pollfd p = {
                    .fd = sock,
                    .events = POLLIN,
                    .revents = 0
                };

                ret = poll(&p, 1, 20);
                if ((ret <= 0) || !(p.revents & POLLIN)) {
                    break;
                }
            }
        }
        close(sock);
    }
}

int main(int argc, char **argv)
{
    char *buf = malloc(BUF_SZ);
    printf("Send cmd: %s\n", argv[1]);
    sprintf(buf, "%s", argv[1]);
    send_pp_cmd(buf, BUF_SZ);
    printf("Reply: %s\n", buf);
}
