/*
 * Copyright 2017 The UBports project
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3, as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranties of
 * MERCHANTABILITY, SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authors:
 *   Marius Gripsgard <marius@ubports.com>
 */

#include "flashlight.h"

#include <gio/gio.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define QCOM_SYSFS "/sys/class/leds/torch-light/brightness"
#define QCOM_ENABLE "255"
#define QCOM_DISABLE "0"

void
toggle_flashlight_action(GAction *action,
                         GVariant *parameter G_GNUC_UNUSED,
                         gpointer data G_GNUC_UNUSED)
{
  GVariant *state;
  gboolean b;
  FILE* fd;

  state = g_action_get_state(action);
  b = g_variant_get_boolean(state);
  g_variant_unref(state);
  fd = fopen(QCOM_SYSFS, "w");
  if (fd != NULL){
      if (b)
        fprintf(fd, QCOM_DISABLE);
      else
        fprintf(fd, QCOM_ENABLE);
      fclose(fd);
      g_action_change_state(action, g_variant_new_boolean(!b));
  }
}

int
flashlight_supported()
{
  return access(QCOM_SYSFS, F_OK ) != -1;
}
