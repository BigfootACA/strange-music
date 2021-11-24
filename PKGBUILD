# Maintainer: BigfootACA <bigfoot@classfun.cn>
pkgname=strange-music
pkgver=0.1
pkgrel=1
pkgdesc="Play strange music"
arch=(x86_64)
url="https://classfun.cn"
license=(GPL3)
depends=(glibc)
makedepends=(gcc glibc make binutils)
optdepends=("systemd: play strange music everyday")
source=(
	LICENSE
	Makefile
	strange-music.c
	strange-music.service
	strange-music.timer
)
md5sums=(SKIP SKIP SKIP SKIP SKIP)

build(){
	make
}

package() {
	make DESTDIR="$pkgdir" install
	install -vDm644 LICENSE -t "$pkgdir/usr/share/licenses/$pkgname"
}
