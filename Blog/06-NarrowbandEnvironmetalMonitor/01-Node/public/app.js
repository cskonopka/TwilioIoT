(function() {
    Pusher.logToConsole = true;

    var serverUrl = "/",
        commands = [],
        pusher = new Pusher('PUSHER_KEY', {
            cluster: 'PUSHER_CLUSTER',
            encrypted: true
        }),
        channel = pusher.subscribe('m2m-commands'),
        commandList = document.getElementById('commands-list'),
        commandTemplate = document.getElementById('command-template');

    channel.bind('new_command', newCommandReceived);

    function newCommandReceived(data) {
        var newCommandHtml = commandTemplate.innerHTML.replace('{{name}}', data.name);
        newCommandHtml = newCommandHtml.replace('{{command}}', data.command);
        var newCommandNode = document.createElement('div');
        newCommandNode.classList.add('command');
        newCommandNode.innerHTML = newCommandHtml;
        commandList.appendChild(newCommandNode);

        var parent = document.getElementById("commands-list");
        var nodesSameClass = parent.getElementsByClassName("command");
        console.log(nodesSameClass.length);

        if (nodesSameClass.length == "6") {
            for (var i = nodesSameClass.length - 2; 0 <= i; i--)
                if (nodesSameClass[i] && nodesSameClass[i].parentElement)
                    nodesSameClass[i].parentElement.removeChild(nodesSameClass[i]);
        }
    }
})();