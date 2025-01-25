function breakText() {
    const h1 = document.querySelector(".container h1")
    const text = h1.textContent;

    let splittedText = text.split("");

    var clutter = "";
    var halfVal = Math.floor(splittedText.length / 2);

    splittedText.forEach((e, index) => {
        if (index < halfVal) {
            clutter += <span class="firstHalf">${e}</span>;

        } else {
            clutter += <span class="SecondHalf">${e}</span>;

        }
    })
    h1.innerHTML = clutter;
}

breakText();

gsap.from(".firstHalf", {
    x: -200,
    opacity: 0,
    duration: 1.2,
    stagger: 0.1,
    ease: "power2.out"
});

gsap.from(".SecondHalf", {
    x: 200,
    opacity: 0,
    duration: 1.2,
    stagger: 0.1,
    ease: "power2.out"
});

gsap.from(".container h1", {
    y: -100,
    opacity: 0,
    duration: 1.5,
    ease: "power3.out"
});

gsap.from(".container p", {
    x: 200, 
    opacity: 0,
    duration: 1.5,
    ease: "power3.out"
});

gsap.from("#nextPageButton", {
    scale: 0.5, 
    opacity: 0,
    duration: 1,
    ease: "back.out(1.5)"
});