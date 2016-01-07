========================================
zdict.qt
========================================

.. image:: http://i.imgur.com/WZwZbkM.gif



Dependencies
========================================

* Qt5
* `QTermWidget <https://github.com/lxde/qtermwidget>`_



Build
========================================

Arch Linux
------------------------------

Install Dependencies
++++++++++++++++++++

.. code-block:: sh

    $ sudo pacman -S qt5-base
    $ git clone https://aur.archlinux.org/qtermwidget-git.git
    $ cd qtermwidget-git
    $ makepkg
    $ sudo pacman -U qtermwidget-git-*.pkg.tar.xz

Build zdict.qt
++++++++++++++++++++

.. code-block:: sh

    $ git clone https://github.com/zdict/zdict.qt
    $ cd zdict.qt
    $ make -j



Usage
========================================

.. code-block:: sh

    $ ./bin/zdict.qt



License
========================================

GPLv3
