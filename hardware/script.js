
const setTimer = () => {
    const timerValue = document.getElementById('timer').value;
    if (timerValue > 0) {
        fetch(`/set_timer?duration=${timerValue}`)
            .then(res => res.text())
            .then(data => {
                document.getElementById('status').innerHTML = data;
                startCountdown(timerValue);
            })
            .catch(console.error);
    } else {
        alert("Please enter a valid time in seconds.");
    }
};

const updateEnergy = () => {
    fetch('/energy')
        .then(res => res.text())
        .then(data => document.getElementById('energy').innerHTML = data)
        .catch(console.error);
};

const startCountdown = (duration) => {
    let remainingTime = duration;
    const countdownInterval = setInterval(() => {
        if (remainingTime > 0) {
            document.getElementById('countdown').innerHTML = `Countdown: ${remainingTime--} seconds`;
        } else {
            document.getElementById('countdown').innerHTML = "Countdown: Finished";
            clearInterval(countdownInterval);
        }
    }, 1000);
};

setInterval(updateEnergy, 1000);