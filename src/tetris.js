/*
TODO:
- wall-kick data

http://gamedev.tutsplus.com/tutorials/implementation/shuffle-bags-making-random-feel-more-random/

http://ielashi.com/el-tetris-an-improvement-on-pierre-dellacheries-algorithm/

https://github.com/ielashi/eltetris/blob/master/src/eltetris.js

http://www.colinfahey.com/tetris/tetris.html

http://stackoverflow.com/questions/7594473/replacement-for-prototype-js-class-system

http://jsperf.com/stacked-array-perf

*/

//Game loop: https://gist.github.com/1579671
(function () {
    var lastTime = 0;
    var vendors = ['ms', 'moz', 'webkit', 'o'];
    var x = 0;
    while(++x<vendors.length && !window.requestAnimationFrame) {
        window.requestAnimationFrame = window[vendors[x] + 'RequestAnimationFrame'];
        window.cancelAnimationFrame = window[vendors[x] + 'CancelAnimationFrame'] || window[vendors[x] + 'CancelRequestAnimationFrame'];
    }

    if (!window.requestAnimationFrame) window.requestAnimationFrame = function (callback, element) {
        var currTime = new Date().getTime();
        var timeToCall = Math.max(0, 16 - (currTime - lastTime));
        var id = window.setTimeout(function () {
            callback(currTime + timeToCall);
        },
        timeToCall);
        lastTime = currTime + timeToCall;
        return id;
    };

    if (!window.cancelAnimationFrame) window.cancelAnimationFrame = function (id) {
        clearTimeout(id);
    };
}());
///////////////////////////////////





var draw_playfield = function (block, block_color) {

    var x, y, cell, output = $('#output'),
        playfield_pointers = [];

    // default block color
    block_color = block_color === undefined ? "empty" : block_color;

    // Remove all children
    output.empty();

    // Draw new cells.
    for (y = 0; y < block.length; y++) {

        playfield_pointers[y] = [];

        var row_len = block[y].length;
        for (x = 0; x < row_len; x++) {

            cell = $('<div>');
            if (block[y][x] > 0) {
                cell.addClass(block_color);
            } else {
                cell.addClass('empty');
            }

            if (x === 0 && y > 0) {
                cell.addClass('clear_left');
            }
            cell.appendTo(output);
            playfield_pointers[y][x] = cell;
        }
    }
    return playfield_pointers;
};

// tetrominoes data
var i = {
    blocks: [0x0F00, 0x2222, 0x00F0, 0x4444],
    color: 'cyan'
};
var j = {
    blocks: [0x8E00, 0x6440, 0x0E20, 0x44C0],
    color: 'blue'
};
var l = {
    blocks: [0x2E00, 0x4460, 0x0E80, 0xC440],
    color: 'orange'
};
var o = {
    blocks: [0x6600, 0x6600, 0x6600, 0x6600],
    color: 'yellow'
};
var s = {
    blocks: [0x6C00, 0x4620, 0x06C0, 0x8c40],
    color: 'green'
};
var t = {
    blocks: [0x4E00, 0x4640, 0x0E40, 0x4C40],
    color: 'purple'
};
var z = {
    blocks: [0xC600, 0x2640, 0x0C60, 0x4C80],
    color: 'red'
};

var height = 22,
    width = 10;
var playfield = [];
var playfield_pointers = [];
var playing = true;

// construct playfield
var build_playfield = function () {
    for (var y = 0; y < height; y++) {
        playfield[y] = new Array(width);
        for (var x = 0; x < width; x++) {
            playfield[y][x] = 0;
        }
    }

    playfield_pointers = draw_playfield(playfield);
};
build_playfield();

var update_playfield_bit = function (x, y, block_color) {
    var tmp = playfield_pointers[y][x];
    if (x === 0 && y > 0) {
        tmp.removeClass().addClass('clear_left').addClass(block_color);
    } else {
        tmp.removeClass().addClass(block_color);
    }
};


// returns piece_coords of current piece
var render_tetra = function (block_type, rotation, x, y) {

    // queue of bits to be rendered
    var piece_coords = [];

    // check if piece is a valid
    var bit, row = 0,
        col = 0,
        piece = block_type.blocks[rotation];

    // loop through tetramino data
    for (bit = 0x8000; bit > 0; bit = bit >> 1) {

        // evaluate iterator on tetromino bit
        if (piece & bit) {
            var _x = x + col;
            var _y = y + row;

            // check if possible to place bit
            if ((_x < 0) || (_x >= width) || (_y < 0) || (_y >= height) || playfield[_y][_x] > 0) {
                // not valid
                piece_coords = [];
                break;

                // queue bit to be rendered
            } else {
                piece_coords.push([_x, _y]);
            }
        }

        // cycle col between 0 to 3
        // if col is 0 then increment row
        if ((col = ++col % 4) === 0 && ++row);
    }

    return piece_coords;
};

var pieces = [];
var randomPiece = function () {
    if (pieces.length === 0) pieces = [i, i, i, i, j, j, j, j, l, l, l, l, o, o, o, o, s, s, s, s, t, t, t, t, z, z, z, z];
    var type = pieces.splice(Math.floor(Math.random() * (pieces.length - 1)), 1)[0]; // remove a single piece
    var wall_kick = type === i ? 1 : 0;
    return {
        type: type,
        wall_kick: wall_kick
    };
};

// game state
var current = {};

var spawn_tetramino = function () {

    var tetra_data = randomPiece();

    current.block_type = tetra_data.type;
    current.x = 3;
    current.y = 0;
    current.rotation = 0;
    current.piece_coords = [];
    current.wall_kick = tetra_data.wall_kick;

    // spawn first block
    // if current.piece_coords is empty, then piece is unable to spawn and it's gameover!
    // http://harddrop.com/wiki/Spawn_Location
    current.piece_coords = render_tetra(current.block_type, current.rotation, current.x, current.y);

    if (current.piece_coords.length > 0) {

        for (var i = 0; i < current.piece_coords.length; i++) {
            var _x = current.piece_coords[i][0];
            var _y = current.piece_coords[i][1];

            update_playfield_bit(_x, _y, current.block_type.color);

        }

        // reset the game!!!
        // TODO: formal gameover
    } else {
        //console.log("game over!");
        build_playfield();
        spawn_tetramino();
    }
};
spawn_tetramino();

//var process_game_state = function (block_type, movement_x, movement_y, movement_rot, piece_coords) {
var process_game_state = function (current, movement_x, movement_y, movement_rot) {



    // Attempt to render the tetramino. render_queue would be empty if it's not possible
    var render_queue = render_tetra(current.block_type, movement_rot, movement_x, movement_y);

    if (render_queue.length <= 0 && movement_rot != current.rotation) {
        //console.log("wall kick!!");
    }




    if (render_queue.length > 0) {

        var render_queue_copy = $.extend(true, [], render_queue);

        // Remove unecessary playfield bits
        for (var i = 0; i < 4; i++) {
            var _x = current.piece_coords[i][0];
            var _y = current.piece_coords[i][1];

            var match = _.find(render_queue_copy, function (element, index) {
                if (element[0] === _x && element[1] === _y) {
                    // remove bit from queue that shouldn't be redrawn
                    render_queue_copy.splice(index, 1);
                    return true;
                }
                return false;
            });

            // remove bit
            if (!match) {
                update_playfield_bit(_x, _y, 'empty');
            }
        }

        // Render queued tetramino bits
        for (i = 0; i < render_queue_copy.length; i++) {

            var x = render_queue_copy[i][0];
            var y = render_queue_copy[i][1];

            update_playfield_bit(x, y, current.block_type.color);

        }

        // commit information on current piece
        current.piece_coords = render_queue;
        current.rotation = movement_rot;
        current.x = movement_x;
        current.y = movement_y;
        return true;
    }
    return false;
};


Mousetrap.bind(['w', 'a', 's', 'd'], function (e, combo) {

    var left = 'a';
    var down = 's';
    var right = 'd';
    var up = 'w';


    // game state
    var key_pressed = combo;
    var movement_x = current.x;
    var movement_y = current.y;
    var movement_rot = current.rotation;
    //var block_type = current.block_type;
    //var piece_coords = current.piece_coords;

    // attempt game state change
    switch (key_pressed) {
        case left:
            --movement_x;
            break;
        case down:
            ++movement_y;
            break;
        case right:
            ++movement_x;
            break;
        case up:
            movement_rot = ++movement_rot % 4;
            break;
        default:
            // do nothing
            break;
    }
    process_game_state(current, movement_x, movement_y, movement_rot);

});


//////// Game loop ///////

var dt = 0;
var idt = 0;
var speed = {
    start: 0.7,
    decrement: 0.005,
    min: 0.2
};

var step = Math.max(speed.min, speed.start - (speed.decrement * height));
var last = new Date().getTime();
var now = last;

var lock_delay_threshold = 2,
    lock_delay_count = 0;

function game_loop() {
    now = new Date().getTime();

    // using requestAnimationFrame have to be able to handle large delta's caused when it 'hibernates' in a background or non-visible tab
    idt = Math.min(1, (now - last) / 1000.0);

    dt = dt + idt;
    if (dt > step) {
        dt = dt - step;

        var movement_x = current.x;
        var movement_y = current.y;
        var movement_rot = current.rotation;
        //var block_type = current.block_type;
        //var piece_coords = current.piece_coords;

        // attempt to move tetramino downwards
        var lol = process_game_state(current, movement_x, ++movement_y, movement_rot);

        // count lock delay
        if (!lol) {
            ++lock_delay_count;
        } else {
            lock_delay_count = 0;
        }

        // lock current block and spawn a new tetramino!
        if (lock_delay_count >= lock_delay_threshold) {

            lock_delay_count = 0;

            // lock block until the end of time
            for (var i = 0; i < 4; i++) {
                var _x = current.piece_coords[i][0];
                var _y = current.piece_coords[i][1];
                playfield[_y][_x] = 1;
            }

            // clear lines if any (2 pass)
            var index = playfield.length;
            var clear_line_queue_count = 0;
            var playfield_index = _.range(index);
            // queue up any lines to be cleared
            // construct list of new row locations of current rows
            // array element would be the old (current) row location
            // and index would be the new location
            while (index-- > 0) {

                var row = playfield[index];

                var flat_row = _.uniq(row);
                if (flat_row.length === 1 && flat_row[0] === 1) {
                    clear_line_queue_count++;
                    playfield_index[index] = -1;
                }
            }
            playfield_index.sort(function (a, b) {
                return a - b;
            });

            // if there are any lines to clear
            // traverse list of new row locations
            if (clear_line_queue_count > 0) {

                index = playfield_index.length;
                while (index-- > 0) {
                    var old_index = playfield_index[index];

                    for (i = 0; i < width; i++) {

                        // copy old row to new index
                        if (old_index > 0) {
                            playfield[index][i] = playfield[old_index][i];

                            var old_pointer = playfield_pointers[old_index][i];

                            playfield_pointers[index][i].attr("class",
                            old_pointer.attr("class"));

                            // fill row with zeros
                        } else {
                            playfield[index][i] = 0;
                            update_playfield_bit(i, index, "empty");
                        }

                    }
                }

            }

            spawn_tetramino();
        }

    }
    last = now;
    requestAnimationFrame(game_loop);

}
game_loop();
