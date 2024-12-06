%bcond_without tests
%bcond_without weak_deps

%global __os_install_post %(echo '%{__os_install_post}' | sed -e 's!/usr/lib[^[:space:]]*/brp-python-bytecompile[[:space:]].*$!!g')
%global __provides_exclude_from ^/opt/ros/rolling/.*$
%global __requires_exclude_from ^/opt/ros/rolling/.*$

Name:           ros-rolling-geometric-shapes
Version:        2.3.1
Release:        1%{?dist}%{?release_suffix}
Summary:        ROS geometric_shapes package

License:        BSD
URL:            http://ros.org/wiki/geometric_shapes
Source0:        %{name}-%{version}.tar.gz

Requires:       assimp-devel
Requires:       boost-devel
Requires:       boost-filesystem
Requires:       eigen3-devel
Requires:       fcl
Requires:       octomap-devel
Requires:       qhull-devel
Requires:       ros-rolling-console-bridge-vendor
Requires:       ros-rolling-eigen-stl-containers
Requires:       ros-rolling-eigen3-cmake-module
Requires:       ros-rolling-geometry-msgs
Requires:       ros-rolling-random-numbers
Requires:       ros-rolling-rclcpp
Requires:       ros-rolling-resource-retriever
Requires:       ros-rolling-rosidl-default-runtime
Requires:       ros-rolling-shape-msgs
Requires:       ros-rolling-visualization-msgs
Requires:       ros-rolling-ros-workspace
BuildRequires:  assimp-devel
BuildRequires:  boost-devel
BuildRequires:  eigen3-devel
BuildRequires:  fcl-devel
BuildRequires:  octomap-devel
BuildRequires:  pkgconfig
BuildRequires:  qhull-devel
BuildRequires:  ros-rolling-ament-cmake
BuildRequires:  ros-rolling-console-bridge-vendor
BuildRequires:  ros-rolling-eigen-stl-containers
BuildRequires:  ros-rolling-eigen3-cmake-module
BuildRequires:  ros-rolling-geometry-msgs
BuildRequires:  ros-rolling-random-numbers
BuildRequires:  ros-rolling-rclcpp
BuildRequires:  ros-rolling-resource-retriever
BuildRequires:  ros-rolling-rosidl-default-generators
BuildRequires:  ros-rolling-shape-msgs
BuildRequires:  ros-rolling-visualization-msgs
BuildRequires:  ros-rolling-ros-workspace
Provides:       %{name}-devel = %{version}-%{release}
Provides:       %{name}-doc = %{version}-%{release}
Provides:       %{name}-runtime = %{version}-%{release}

%if 0%{?with_tests}
BuildRequires:  ros-rolling-ament-cmake-copyright
BuildRequires:  ros-rolling-ament-cmake-gtest
BuildRequires:  ros-rolling-ament-lint-auto
BuildRequires:  ros-rolling-ament-lint-cmake
%endif

%description
This package contains generic definitions of geometric shapes and bodies.

%prep
%autosetup -p1

%build
# In case we're installing to a non-standard location, look for a setup.sh
# in the install tree and source it.  It will set things like
# CMAKE_PREFIX_PATH, PKG_CONFIG_PATH, and PYTHONPATH.
if [ -f "/opt/ros/rolling/setup.sh" ]; then . "/opt/ros/rolling/setup.sh"; fi
mkdir -p .obj-%{_target_platform} && cd .obj-%{_target_platform}
%cmake3 \
    -UINCLUDE_INSTALL_DIR \
    -ULIB_INSTALL_DIR \
    -USYSCONF_INSTALL_DIR \
    -USHARE_INSTALL_PREFIX \
    -ULIB_SUFFIX \
    -DCMAKE_INSTALL_PREFIX="/opt/ros/rolling" \
    -DAMENT_PREFIX_PATH="/opt/ros/rolling" \
    -DCMAKE_PREFIX_PATH="/opt/ros/rolling" \
    -DSETUPTOOLS_DEB_LAYOUT=OFF \
%if !0%{?with_tests}
    -DBUILD_TESTING=OFF \
%endif
    ..

%make_build

%install
# In case we're installing to a non-standard location, look for a setup.sh
# in the install tree and source it.  It will set things like
# CMAKE_PREFIX_PATH, PKG_CONFIG_PATH, and PYTHONPATH.
if [ -f "/opt/ros/rolling/setup.sh" ]; then . "/opt/ros/rolling/setup.sh"; fi
%make_install -C .obj-%{_target_platform}

%if 0%{?with_tests}
%check
# Look for a Makefile target with a name indicating that it runs tests
TEST_TARGET=$(%__make -qp -C .obj-%{_target_platform} | sed "s/^\(test\|check\):.*/\\1/;t f;d;:f;q0")
if [ -n "$TEST_TARGET" ]; then
# In case we're installing to a non-standard location, look for a setup.sh
# in the install tree and source it.  It will set things like
# CMAKE_PREFIX_PATH, PKG_CONFIG_PATH, and PYTHONPATH.
if [ -f "/opt/ros/rolling/setup.sh" ]; then . "/opt/ros/rolling/setup.sh"; fi
CTEST_OUTPUT_ON_FAILURE=1 \
    %make_build -C .obj-%{_target_platform} $TEST_TARGET || echo "RPM TESTS FAILED"
else echo "RPM TESTS SKIPPED"; fi
%endif

%files
/opt/ros/rolling

%changelog
* Fri Dec 06 2024 Tyler Weaver <tyler@picknik.ai> - 2.3.1-1
- Autogenerated by Bloom

* Fri Nov 29 2024 Tyler Weaver <tyler@picknik.ai> - 2.3.0-1
- Autogenerated by Bloom

* Wed Jun 26 2024 Tyler Weaver <tyler@picknik.ai> - 2.2.1-1
- Autogenerated by Bloom

* Sat Jun 08 2024 Tyler Weaver <tyler@picknik.ai> - 2.2.0-1
- Autogenerated by Bloom

* Thu Mar 07 2024 Tyler Weaver <tyler@picknik.ai> - 2.1.3-4
- Autogenerated by Bloom

* Tue Mar 21 2023 Tyler Weaver <tyler@picknik.ai> - 2.1.3-3
- Autogenerated by Bloom

