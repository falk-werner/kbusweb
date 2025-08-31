# -*-makefile-*-
#
# Copyright (C) 2025 by <Falk Werner>
#
# For further information about the PTXdist project and license conditions
# see the README file.
#

#
# We provide this package
#
PACKAGES-$(PTXCONF_KBUSWEB) += kbusweb

#
# Paths and names
#
KBUSWEB_VERSION	:= 1.0.0
KBUSWEB		:= kbusweb-$(KBUSWEB_VERSION)
KBUSWEB_URL		:= file://local_src/kbusweb
KBUSWEB_DIR		:= $(BUILDDIR)/$(KBUSWEB)
KBUSWEB_LICENSE	:= unknown

# ----------------------------------------------------------------------------
# Get
# ----------------------------------------------------------------------------

#$(KBUSWEB_SOURCE):
#	@$(call targetinfo)
#	@$(call get, KBUSWEB)

# ----------------------------------------------------------------------------
# Prepare
# ----------------------------------------------------------------------------

#KBUSWEB_CONF_ENV	:= $(CROSS_ENV)

#
# cmake
#
KBUSWEB_CONF_TOOL	:= cmake
#KBUSWEB_CONF_OPT	:= $(CROSS_CMAKE_USR)

# ----------------------------------------------------------------------------
# Target-Install
# ----------------------------------------------------------------------------

$(STATEDIR)/kbusweb.targetinstall:
	@$(call targetinfo)

	@$(call install_init, kbusweb)
	@$(call install_fixup, kbusweb, PRIORITY, optional)
	@$(call install_fixup, kbusweb, SECTION, base)
	@$(call install_fixup, kbusweb, AUTHOR, "<Falk Werner>")
	@$(call install_fixup, kbusweb, DESCRIPTION, missing)

#	# This is an example only. Adapt it to your requirements. Read the
#	# documentation's section "Make it Work" in chapter "Adding new Packages"
#	# how to prepare this content or/and read chapter
#	# "Rule File Macro Reference" to get an idea of the available macros
#	# you can use here and how to use them.

	@$(call install_copy, kbusweb, 0, 0, 0755, -, /usr/bin/kbusweb)

	@$(call install_finish, kbusweb)

	@$(call touch)

# ----------------------------------------------------------------------------
# Clean
# ----------------------------------------------------------------------------

#$(STATEDIR)/kbusweb.clean:
#	@$(call targetinfo)
#	@$(call clean_pkg, KBUSWEB)

# vim: syntax=make
